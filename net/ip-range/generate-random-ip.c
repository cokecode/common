#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

#define MAX_NUMBER (64 * 1024)

struct ipinfo {
    uint32_t ip;
    uint32_t netmask;
};

/**
 * Goodclients.conf file format:
 * x.x.x.x/netmask ...
 */
int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Generate random IP from goodclients config file.\n");
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "   %s goodclients.conf\n", argv[0]);
        return 1;
    }

    const char * filepath = argv[1];
    FILE * fp;
    char buf[1024];
    uint32_t ip;
    int netmask = 0;
    char * p;
    struct ipinfo array[MAX_NUMBER];
    int i = 0;
    int count = 0;
    int n = 20;
    int total = 0;

    if (argc > 2)
        n = atoi(argv[2]);

    fp = fopen(filepath, "r");
    while (fgets(buf, sizeof(buf), fp) != NULL && i < MAX_NUMBER) {
        if ((p = strchr(buf, '/')) != NULL) {
            *p = '\0';
            p++;
            
            inet_pton(AF_INET, buf, &ip);
            ip = ntohl(ip);
            netmask = atoi(p);
            // printf("==> %08X / %d\n", ip, netmask);

            array[i].ip = ip;
            array[i].netmask = netmask;
            i++;
        }
    }
    total = i;
    fclose(fp); fp = NULL;

    srand(time(NULL));

    while (count < n) {
        struct ipinfo * x = &array[rand() % total];
        uint32_t mask = (1 << (32 - x->netmask)) - 1; // we got 0xfff..
        uint32_t ip = x->ip + rand() % mask;
        ip = htonl(ip);
        inet_ntop(AF_INET, &ip, buf, sizeof(buf));
        printf("%s\n", buf);
        count++;
    }

    return 0;
}
