#include "cstrutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *copy = strdup("> foo  \nabcd\n4321\n>  bar\nxyz\n");
    struct cursor x = new_cursor(copy);

    struct cursor header_start, header_end;
    x = mark(skip_s(inc(next_c(x, '>')), " \t"), &header_start);
    struct cursor seq = x = inc(mark(next_c(x, '\n'), &header_end));

    struct cursor header = strip(header_start, header_end);
    printf("header: '%s'\n", header.x);

    x = mark(skip_s(inc(next_c(x, '>')), " \t"), &header_start);
    x = inc(mark(next_c(x, '\n'), &header_end));

    header_start = strip(header_start, header_end);
    printf("header: '%s'\n", header_start.x);

    free(copy);

    return 0;
}
