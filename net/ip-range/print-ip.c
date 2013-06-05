#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    uint32_t ip;
    int i;

    if (inet_pton(AF_INET, argv[1], &ip) == 1) {
        printf("Parsed IP OK.\n");
        ip = ntohl(ip);
        printf("%u\n", ip);
        printf("%08X\n", ip);

        for (i = 31; i >= 0; i--) {
            if (ip & (1 << i))
                putchar('1');
            else
                putchar('0');
            if (i > 0 && i % 8 == 0)
                putchar(' ');
        }
        putchar('\n');
    }
    return 0;
}
