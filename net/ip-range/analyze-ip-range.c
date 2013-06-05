#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <arpa/inet.h>

int verbose = 0;
int expand_count = 0;

void debug(const char* format, ...)
{
    if (verbose) {
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stdout, format, argptr);
        va_end(argptr);
    }
}

void print_ip(uint32_t a, uint32_t b, int netmask)
{
    char buf1[1024], buf2[1024];
    int n;

    if (b > a) {
        n = htonl(a);
        inet_ntop(AF_INET, &n, buf1, sizeof(buf1));
        n = htonl(b);
        inet_ntop(AF_INET, &n, buf2, sizeof(buf2));
        if (verbose) {
            printf("  { %s, %s } / %d | %08X, %08X delta = %08X\n", buf1, buf2, netmask, a, b, b - a);
            printf("%s/%d\n", buf1, netmask);
        }
        else {
            printf("%s/%d\n", buf1, netmask);
        }
        expand_count++;
    }
    else {
        assert(b == 0);
        n = htonl(a);
        inet_ntop(AF_INET, &n, buf1, sizeof(buf1));
        if (verbose) {
            printf("  { %s } | %08X\n", buf1, a);
        }
        else {
            if (netmask > 0)
                printf("%s/%d\n", buf1, netmask);
            else
                printf("%s\n", buf1);
        }
        expand_count++;
    }
}

void print_ip2(uint32_t a, uint32_t b)
{
    char buf1[1024], buf2[1024];
    int n;

    n = htonl(a);
    inet_ntop(AF_INET, &n, buf1, sizeof(buf1));
    n = htonl(b);
    inet_ntop(AF_INET, &n, buf2, sizeof(buf2));
    if (verbose)
        printf("  -- { %s, %s } --\n", buf1, buf2);
}

void foo(uint32_t a, uint32_t b)
{
    int count = 0;
    uint32_t next_edge;
    uint32_t delta = b - a;

    if (a == b) {
        debug("## switch0\n");
        print_ip(a, 0, 0);
        return;
    }

    print_ip2(a, b);

    // x.x.x.0 ~ x.x.x.255, or x.x.0.0 ~ x.x.255.255, or so
    if ((delta & (delta + 1)) == 0) {
        debug("## switch1\n");
        int netmask = 32;
        delta = delta + 1;
        do {
            delta >>= 1;
            netmask--;
        } while (delta != 1);   // XXX delta != 0??
        //print_ip(a, b, netmask);
        if (a % 0x10000 == 0) {
            for (; a < b; a += 0x10000) {
                print_ip(a, 0, 16);
            }
            return;
        }
        if (a % 0x100 == 0) {
            for (; a < b; a += 0x100) {
                print_ip(a, 0, 24);
            }
            return;
        }
        assert(0);
        return;
    }

    // { 1.27.112.0, 1.31.255.255 } | 11FFFFF - 11B7000 = 48FFF
    // ==>
    // { 1.27.112.0, 1.27.112.255 }
    // { 1.27.113.0, 1.27.113.255 }
    // .....
    // { 1.27.255.0, 1.27.255.255 }
    // { 1.28.0.0,   1.28.255.255 }
    // { 1.29.0.0,   1.29.255.255 }
    // { 1.30.0.0,   1.30.255.255 }
    // { 1.31.0.0,   1.31.255.255 }
    if (a % 0x100 == 0 && (delta + 1) % 0x100 == 0 && delta > 0x10000) {
        debug("## switch1.5\n");
        next_edge = (a / 0x10000 + 1) * 0x10000 - 1;

        // x.x.N.0 ~ x.x.255.255
        foo(a, next_edge);

        a = next_edge + 1;

        // x.N.0.0 ~ x.M.255.255
        while (a + 0x10000 < b) {
            foo(a, a + 0x10000 - 1);
            a += 0x10000;
        }

        // x.x.0.0 ~ x.x.N.M
        foo(a, b);
        return;
    }

    // ==> { 1.1.8.0, 1.1.31.255 } | 1011FFF - 1010800 = 17FF
    if (a % 0x100 == 0 && (delta + 1) % 0x100 == 0) {
        debug("## switch2\n");
        while (a < b) {
            print_ip(a, a + 0x100 - 1, 24);
            a += 0x100;
            count++;
        }
        debug("  count = %d\n", count);
        assert(count <= 255);
        return;
    }

    // { 1.24.32.0, 1.24.66.227 } | 11842E3 - 1182000 = 22E3
    // ==>
    // { 1.24.32.0, 1.24.65.255 } +
    // { 1.24.66.0, 1.24.66.227 }
    if (a % 0x100 == 0 && delta > 0x100) {
        debug("## switch3\n");
        foo(a, a + delta / 0x100 * 0x100 - 1);
        foo(a + delta / 0x100 * 0x100, b);
        return;
    }

    // x.x.x.0 ~ x.x.x.N (N < 255)
    if (a % 0x100 == 0) {
        debug("## switch4\n");
        a += 1; // to avoid x.x.x.0, this is a network addr, not a single IP addr.
        while (a <= b) {
            print_ip(a, 0, 0);
            a++;
            count++;
        }
        debug("  count = %d\n", count);
        assert(count < 255);
        return;
    }

    // { 1.24.66.229, 1.24.70.103 } | 1184667 - 11842E5 = 382
    // ==>
    // { 1.24.66.229, 1.24.66.255 } +
    // { 1.24.67.0,   1.24.69.255 } +
    // { 1.24.70.0,   1.24.70.103 }
    if (a % 0x100 != 0 && delta < 0x100) {
        debug("## switch5\n");

        next_edge = (a / 0x100 + 1) * 0x100 - 1;

        if (b <= next_edge) {
            // x.x.x.N ~ x.x.x.255, or
            // x.x.x.N ~ x.x.x.M
            debug("## switch5.1\n");
            while (a <= b) {
                print_ip(a, 0, 0);
                a++;
                count++;
            }
            debug("  count = %d\n", count);
            assert(count < 255);
        }
        else {
            debug("## switch5.2\n");
            foo(a, next_edge);

            a = next_edge + 1;

            // x.x.x.0 ~ x.x.y.N, or
            // x.x.x.0 ~ x.x.x.N
            while (a + 0x100 < b) {
                foo(a, a + 0x100 - 1);
                a += 0x100;
                count++;
            }
            debug("  count = %d\n", count);
            assert(count < 255);

            // x.x.x.0 ~ x.x.x.N
            foo(a, b);
        }
        return;
    }

    /*
    case 1
        { 1.51.0.1, 1.51.15.255 } | 1330FFF - 1330001 = FFE
        ==>
        { 1.51.0.1, 1.51.0.255 }
        { 1.51.1.0, 1.51.1.255 }
        { 1.51.2.0, 1.51.2.255 }
        .....
        { 1.51.15.0, 1.51.15.255 }
    */
    /*
    case 2
    ==> { 175.68.145.79, 175.88.31.255 } | AF581FFF - AF44914F = 138EB0
      -- { 175.68.145.79, 175.88.31.255 } --
    ## switch6
      -- { 175.68.145.79, 175.68.145.255 } --
    ## switch5
    ## switch5.1
      { 175.68.145.79 } | AF44914F
      { 175.68.145.80 } | AF449150
      { 175.68.145.81 } | AF449151
      { 175.68.145.82 } | AF449152
      { 175.68.145.83 } | AF449153
      { 175.68.145.84 } | AF449154
    */
    if (a % 0x100 != 0 && delta >= 0x100) {
        debug("## switch6\n");
        next_edge = (a / 0x100 + 1) * 0x100 - 1;

        // x.x.x.N ~ x.x.x.255
        foo(a, next_edge);
        
        debug("## switch6.back1\n");

        a = next_edge + 1;

        if (delta > 0x10000) {
            // x.N.0.0 ~ x.M.255.255
            while (a + 0x10000 < b) {
                foo(a, a + 0x10000 - 1);
                a += 0x10000;
                count++;
            }
            debug("  count = %d\n", count);
            assert(count < 255);   // XXX
        }
        else {
            // x.x.N.0 ~ x.x.M.255
            while (a + 0x100 < b) {
                foo(a, a + 0x100 - 1);
                a += 0x100;
                count++;
            }
            debug("  count = %d\n", count);
            assert(count < 1000);   // XXX
        }

        debug("## switch6.back2\n");
        
        // x.x.x.0 ~ x.x.x.N
        foo(a, b);
        return;
    }

    debug("ERROR\n");
}

int main(int argc, char *argv[])
{
    char s1[1024], s2[1024];
    uint32_t a, b;
    int corrected = 0;

    if (argc > 1 && strcmp(argv[1], "--verbose") == 0) {
        verbose = 1;
    }

    // while (scanf("%u %u %s %s", &a, &b, s1, s2) == 4) {
    while (scanf("%s %s", s1, s2) == 2) {
        inet_pton(AF_INET, s1, &a);
        inet_pton(AF_INET, s2, &b);
        a = ntohl(a);
        b = ntohl(b);

        expand_count = 0;
        debug("==> { %s, %s } | %X - %X = %X\n", s1, s2, b, a, b - a);

        // skip 255.255.255.0
        if (a == 0xffffff00)
            continue;

        if (a == b && a % 0x100 == 0) {
            debug("  empty network, ignore.\n");
            continue;
        }

        corrected = 0;
        if ((a - 1) % 0x100 == 0) {
            // fix some ip range like (x.x.x.1 ~ x.x.x.255), it is same to (x.x.x.0 ~ x.x.x.255)
            a -= 1;
            corrected = 1;
        }
        if (b % 0x100 == 0) {
            // fix: x.x.3.0 -> x.x.2.255
            b -= 1;
            corrected = 1;
        }

        if (corrected) {
            uint32_t A = htonl(a), B = htonl(b);
            inet_ntop(AF_INET, &A, s1, sizeof(s1));
            inet_ntop(AF_INET, &B, s2, sizeof(s2));
            debug("*** { %s, %s } (corrected)\n", s1, s2);
            // assert(b > a);
        }

        foo(a, b);

        debug("==> expand_count = %d\n", expand_count);
        debug("\n");
    }
    return 0;
}
