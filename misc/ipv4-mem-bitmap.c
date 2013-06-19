#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Memory bitmap for IPv4, every IP has 2 bits in it. */

#define BITMAP_SIZE ((1ULL << 32) / 8 * 2)

int main()
{
    unsigned char * bitmap;
    uint32_t ip = 12345678;
    unsigned char * p;

    //printf("size of size_t = %lu\n", sizeof(size_t));

    bitmap = malloc(BITMAP_SIZE);
    if (bitmap == NULL) {
        perror("malloc failed");
        exit(1);
    }

    memset(bitmap, 0, BITMAP_SIZE);

    printf("memset bitmap ok, bitmap size = %llu\n", BITMAP_SIZE);

    uint32_t bytes_offset = ip / 8 * 2 + (ip % 8) * 2 / 8;
    uint32_t bits_offset = (ip % 8) * 2 % 8;

    printf("bytes_offset = %d\n", bytes_offset);
    printf("bits_offset  = %d\n", bits_offset);

    p = &bitmap[bytes_offset];
    printf("bitmap addr: %p\n", bitmap);
    printf("p      addr: %p\n", p);
    printf("*p = 0x%02X\n", *p);

    unsigned char flag1 = 1 << bits_offset;
    unsigned char flag2 = 1 << (bits_offset + 1);
    printf("flag1 = 0x%02X\n", flag1);
    printf("flag2 = 0x%02X\n", flag2);

    *p |= flag1;
    printf("set flag1 bit: *p = 0x%02X\n", *p);

    *p |= flag2;
    printf("set flag2 bit: *p = 0x%02X\n", *p);

    printf("flag1 set? %d\n", !!(*p & flag1));
    printf("flag2 set? %d\n", !!(*p & flag2));

    *p &= ~flag1;
    printf("clear flag1 bit: *p = 0x%02X\n", *p);

    *p &= ~flag2;
    printf("clear flag2 bit: *p = 0x%02X\n", *p);

    return 0;
}
