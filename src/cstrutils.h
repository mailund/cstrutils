#ifndef CSTRUTILS_H
#define CSTRUTILS_H

// unless we include from cstrutils.c, inline means inline.
// inside cstrutils.c we define the functions for cases where they
// are not inlined.
#ifndef INLINE
#define INLINE extern inline
#endif

#define DISPATCH(FNAME, X)    \
    _Generic(X,               \
             const char *     \
             : const_##FNAME, \
               char *         \
             : mutable_##FNAME)

enum state
{
    EOS = 1 << 0,
    MATCH_FAILURE = 1 << 1,
};
struct cursor
{
    char *x;
    enum state state;
};
struct const_cursor
{
    const char *x;
    enum state state;
};

#define PROP_ERROR(CUR)   \
    if ((CUR).state != 0) \
        return CUR;

INLINE struct const_cursor const_new_cursor(const char *x)
{
    return (struct const_cursor){.x = x, .state = *x ? 0 : EOS};
}
INLINE struct cursor mutable_new_cursor(char *x)
{
    return (struct cursor){.x = x, .state = *x ? 0 : EOS};
}
#define new_cursor(X)       \
    DISPATCH(new_cursor, X) \
    (X)

// Increment to the next character, except if we
// are already at the end of the string.
INLINE struct const_cursor const_inc(struct const_cursor cur);
INLINE struct cursor mutable_inc(struct cursor cur);
#define inc(CUR)           \
    DISPATCH(inc, (CUR).x) \
    (CUR)

// Move cursor to the next occurrence of ch
INLINE struct const_cursor const_next_c(struct const_cursor cur, char ch);
INLINE struct cursor mutable_next_c(struct cursor cur, char ch);
#define next_c(CUR, CH)       \
    DISPATCH(next_c, (CUR).x) \
    (CUR, CH)

// Move cursor to the next occurrence of an element in set
INLINE struct const_cursor const_next_s(struct const_cursor cur, const char *set);
INLINE struct cursor mutable_next_s(struct cursor cur, const char *set);
#define next_s(CUR, S)        \
    DISPATCH(next_s, (CUR).x) \
    (CUR, S)

INLINE struct const_cursor const_inc(struct const_cursor cur)
{
    PROP_ERROR(cur);
    cur.x++;
    return cur;
}
INLINE struct cursor mutable_inc(struct cursor cur)
{
    PROP_ERROR(cur);
    cur.x++;
    return cur;
}

INLINE struct const_cursor const_next_c(struct const_cursor cur, char ch)
{
    PROP_ERROR(cur);
    const char *x = cur.x;
    for (; *x && *x != ch; x++)
        ;
    return new_cursor(x);
}
INLINE struct cursor mutable_next_c(struct cursor cur, char ch)
{
    PROP_ERROR(cur);
    char *x = cur.x;
    for (; *x && *x != ch; x++)
        ;
    return new_cursor(x);
}

INLINE struct const_cursor const_next_s(struct const_cursor cur, const char *set)
{
    PROP_ERROR(cur);
    const char *x = cur.x;
    for (; *x; x++)
    {
        for (const char *ch = set; *ch; ch++)
        {
            if (*x == *ch)
                goto done;
        }
    }
done:
    return new_cursor(x);
}
INLINE struct cursor mutable_next_s(struct cursor cur, const char *set)
{
    PROP_ERROR(cur);
    char *x = cur.x;
    for (; *x; x++)
    {
        for (const char *ch = set; *ch; ch++)
        {
            if (*x == *ch)
                goto done;
        }
    }
done:
    return new_cursor(x);
}
#define next_s(CUR, S)        \
    DISPATCH(next_s, (CUR).x) \
    (CUR, S)

// FIXME: dispatch on both types!
INLINE struct const_cursor const_mark(struct const_cursor cur,
                                      struct const_cursor *mark)
{
    PROP_ERROR(cur);
    *mark = new_cursor(cur.x);
    return cur;
}
INLINE struct cursor mutable_mark(struct cursor cur,
                                  struct cursor *mark)
{
    PROP_ERROR(cur);
    *mark = new_cursor(cur.x);
    return cur;
}
#define mark(CUR, P)        \
    DISPATCH(mark, (CUR).x) \
    (CUR, P)

INLINE struct const_cursor const_skip_s(struct const_cursor cur, const char *set)
{
    PROP_ERROR(cur);
    const char *x = cur.x;
    for (; *x; x++)
    {
        for (const char *ch = set; *ch; ch++)
        {
            if (*x == *ch)
                goto next_x_pos;
        }
        goto done; // We didn't have a match, so we are done
    next_x_pos:;   // Go to next iteration of outer loop
    }
done:
    return new_cursor(x);
}
INLINE struct cursor mutable_skip_s(struct cursor cur, const char *set)
{
    PROP_ERROR(cur);
    char *x = cur.x;
    for (; *x; x++)
    {
        for (const char *ch = set; *ch; ch++)
        {
            if (*x == *ch)
                goto next_x_pos;
        }
        goto done; // We didn't have a match, so we are done
    next_x_pos:;   // Go to next iteration of outer loop
    }
done:
    return new_cursor(x);
}
#define skip_s(CUR, S)        \
    DISPATCH(skip_s, (CUR).x) \
    (CUR, S)

struct cursor skip_back(struct cursor beg,
                        struct cursor end,
                        const char *set);
struct cursor nul(struct cursor cur);

struct cursor strip(struct cursor beg, struct cursor end);
struct cursor compact(struct cursor cur,
                      struct cursor *dest,
                      char term,
                      const char *skip);

#endif // CSTRUTILS_H
