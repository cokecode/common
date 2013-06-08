#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    uint32_t a, b, ip, tmp;
    char buf1[128], buf2[128];
    char buf[128];
    uint32_t i = 0, count, total = 0;
    FILE *fp;

    while (scanf("%s %s", buf1, buf2) == 2) {
        inet_pton(AF_INET, buf1, &a);
        inet_pton(AF_INET, buf2, &b);
        a = ntohl(a);
        b = ntohl(b);
        if (a == 0xffffff00)
            continue;
        
        // printf("==> %s ~ %s, %08X, %08X\n", buf1, buf2, a, b);
        i++;
        sprintf(buf, "(%s, %s)", buf1, buf2);
        fprintf(stderr, "%6d: %-40s | ", i, buf);

        sprintf(buf, "ALLIP.%s.%s.txt", buf1, buf2);
        fp = fopen(buf, "w");

        count = 0;
        for (ip = a; ip <= b; ip++) {
            tmp = htonl(ip);
            inet_ntop(AF_INET, &tmp, buf1, sizeof(buf1));
            fprintf(fp, "%s\n", buf1);
            count++;
        }
        total += count;

        fprintf(stderr, "%8d\n", count);
        fclose(fp);
    }

    fprintf(stderr, "--\n");
    fprintf(stderr, "%u IP ranges, %u IPs.\n", i, total);

    return 0;
}
