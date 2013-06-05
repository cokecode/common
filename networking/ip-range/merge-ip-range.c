#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    uint32_t a, b, lasta = 0, lastb = 0;
    char str1[128], str2[128];
    char laststr1[128], laststr2[128];

    while (scanf("%s %s", str1, str2) == 2) {
        inet_pton(AF_INET, str1, &a);
        inet_pton(AF_INET, str2, &b);   // XXX check?
        a = ntohl(a);
        b = ntohl(b);

        if (a == lastb || a == lastb + 1) {
            lastb = b;
            strncpy(laststr2, str2, sizeof(laststr2));
        }
        else {
            if (lasta) {
                printf("%s %s\n", laststr1, laststr2);
            }
            lasta = a;
            lastb = b;
            strncpy(laststr1, str1, sizeof(laststr1));
            strncpy(laststr2, str2, sizeof(laststr2));
        }
    }

    if (lasta) {
        printf("%s %s\n", laststr1, laststr2);
    }
}
