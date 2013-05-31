#include <stdio.h>
#include <string.h>

int parse_string(const char * str)
{
    int offset;
    char buf[1024];
    char * p;
    char * ip, * mask, * attr;

    strncpy(buf, str, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    p = buf;

    /* expect IP */
    ip = p;
    offset = strspn(p, "0123456789.");
    if (offset == 0)
        return -1;

    p += offset;
    if (*p == '/') { /* with network mask */
        *p = 0;
        p++;

        mask = p;
        offset = strspn(p, "0123456789");
        if (offset == 0)
            return -1;

        p += offset;
    }
    else { /* single ip, continue */
        mask = NULL;
    }

    /* expect blank before attr */
    offset = strspn(p, " \t");
    if (offset == 0)
        return -1;

    *p = '\0';  /* terminate privious string with null-character. */
    p += offset;
    attr = p;

    printf("<%s> <%s> <%s>\n", ip, mask, attr);

    return 0;
}

int main()
{
    parse_string("192.168.2.100/24 white");
    parse_string("192.168.2.100 white");
    parse_string("192.168.2.100");
    parse_string("/24 gray");
    parse_string("1.2.3 gray");
    return 0;
}
