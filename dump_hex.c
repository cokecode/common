#include <stdlib.h>
#include <stdio.h>

void dump_hex(unsigned char * data, size_t size)
{
	static char HEX[] = "0123456789ABCDEF";
	int i;

	for (i = 0; i < size; i++) {
		fputc(HEX[(data[i] >> 4) & 0x0F], stdout);
		fputc(HEX[(data[i] >> 0) & 0x0F], stdout);

		if (i && (i + 1) % 16 == 0) {
			fputc('\n', stdout);
		}
		else if (i && (i + 1) % 8 == 0) {
			fputc(' ', stdout);
			fputc('-', stdout);
			fputc(' ', stdout);
		}
		else if (i && (i + 1) % 4 == 0) {
			fputc(' ', stdout);
			fputc(' ', stdout);
		}
		else {
			fputc(' ', stdout);
		}
	}
}

/*
int main()
{
	char a[2];
	dump_hex("ABCDEF\0255", 7);
	putchar('\n');
	a[0] = 127;
	a[1] = 255;
	dump_hex(a, 2);
	putchar('\n');
}
*/
