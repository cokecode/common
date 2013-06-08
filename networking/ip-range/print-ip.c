#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

/* IP should be network-byteorder */
void print_ip(uint32_t ip)
{
    char buf[1024];
    uint32_t num;
    int i;

    num = ntohl(ip);
    printf("%u\n", num);
    printf("%08X\n", num);

    for (i = 31; i >= 0; i--) {
        if (num & (1 << i))
            putchar('1');
        else
            putchar('0');
        if (i > 0 && i % 8 == 0)
            putchar(' ');
    }
    putchar('\n');

    if (inet_ntop(AF_INET, &ip, buf, sizeof(buf)) != NULL) {
        printf("%s\n", buf);
    }
    else {
        printf("Invalid IP.\n");
    }
}

int main(int argc, char *argv[])
{
    uint32_t ip;
    const char * arg = argv[1];

    if (inet_pton(AF_INET, arg, &ip) == 1) { /* dotted decimal */
        printf("Parsed IP OK.\n");
        print_ip(ip);
    }
    else {
        /* try number */
        ip = strtoul(arg, NULL, 10);
        if (ip > 0) {
            print_ip(htonl(ip));
            return 0;
        }

        /* try hexcimal */
        ip = strtoul(arg, NULL, 16);
        if (ip > 0) {
            print_ip(htonl(ip));
            return 0;
        }
    }

    return 0;
}
