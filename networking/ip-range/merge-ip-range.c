#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    uint32_t a, b, lasta = 0, lastb = 0;
    uint32_t tmp;
    char str1[128], str2[128];
    char laststr1[128], laststr2[128];
    int ip_is_number = 0;
    uint32_t total = 0;

    if (argc > 1 && strcmp(argv[1], "--number") == 0) {
        ip_is_number = 1;
    }

    while (scanf("%s %s", str1, str2) == 2) {
        if (ip_is_number) {
            a = strtoul(str1, NULL, 0);
            b = strtoul(str2, NULL, 0);
            tmp = htonl(a); inet_ntop(AF_INET, &tmp, str1, sizeof(str1));
            tmp = htonl(b); inet_ntop(AF_INET, &tmp, str2, sizeof(str2));
        }
        else {
            inet_pton(AF_INET, str1, &a);
            inet_pton(AF_INET, str2, &b);   // XXX check?
            a = ntohl(a);
            b = ntohl(b);
        }

        if (a == lastb || a == lastb + 1) {
            lastb = b;
            strncpy(laststr2, str2, sizeof(laststr2));
        }
        else {
            if (lasta) {
                printf("%s %s\n", laststr1, laststr2);
                total += lastb - lasta + 1;
            }
            lasta = a;
            lastb = b;
            strncpy(laststr1, str1, sizeof(laststr1));
            strncpy(laststr2, str2, sizeof(laststr2));
        }
    }

    if (lasta) {
        printf("%s %s\n", laststr1, laststr2);
        total += lastb - lasta + 1;
    }

    fprintf(stderr, "Total: %u IPs\n", total);
    return 0;
}
