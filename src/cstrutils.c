
#define INLINE inline
#include "cstrutils.h"

#include <stdio.h>

struct cursor skip_back(struct cursor beg,
                        struct cursor end,
                        const char *set)
{
    while (beg.x < end.x)
    {
        end.x--;
        for (const char *ch = set; *ch; ch++)
        {
            if (*end.x == *ch)
            {
                goto cont;
            }
        }
        // we don't match a character from set, so we should
        // return one-past this one.
        end.x++;
        break;
    cont:;
    }
    return end;
}

struct cursor nul(struct cursor cur)
{
    PROP_ERROR(cur);
    *cur.x = '\0';
    return cur;
}

struct cursor strip(struct cursor beg, struct cursor end)
{
    PROP_ERROR(beg);
    PROP_ERROR(end);
    nul(skip_back(beg, end, " \t"));
    return skip_s(beg, " \t");
}

struct cursor compact(struct cursor cur,
                      struct cursor *dest,
                      char term,
                      const char *skip)
{
    PROP_ERROR(cur);
    char *x = cur.x, *y = cur.x;
    for (; *x && *x != term; x++)
    {
        for (const char *ch = skip; *ch; ch++)
        {
            if (*x == *ch)
                goto skip_to_next;
        }
        *y++ = *x;
    skip_to_next:;
    }

    if (x != y)
    {
        *y = '\0'; // terminate compated (if not the full range)
    }
    *dest = new_cursor(y);
    return new_cursor(x);
}
