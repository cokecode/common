#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

static int HexPairValue(const char * code)
{
	int value = 0;
	const char * pch = code;
	for (;;) {
		int digit = *pch++;
		if (digit >= '0' && digit <= '9') {
			value += digit - '0';
		}
		else if (digit >= 'A' && digit <= 'F') {
			value += digit - 'A' + 10;
		}
		else if (digit >= 'a' && digit <= 'f') {
			value += digit - 'a' + 10;
		}
		else {
			return -1;
		}
		if (pch == code + 2)
			return value;
		value <<= 4;
	}
}

int UrlDecode(const char *source, char *dest)
{
	char * start = dest;

	while (*source) {
		switch (*source) {
		case '+':
			*(dest++) = ' ';
			break;
		case '%':
			if (source[1] && source[2]) {
				int value = HexPairValue(source + 1);
				if (value >= 0) {
					*(dest++) = value;
					source += 2;
				}
				else {
					*dest++ = '?';
				}
			}
			else {
				*dest++ = '?';
			}
			break;
		default:
			*dest++ = *source;
		}
		source++;
	}

	*dest = 0;
	return dest - start;
}  

int UrlEncode(const char *source, char *dest, unsigned max)  
{
	static const char *digits = "0123456789ABCDEF";
	unsigned char ch;
	unsigned len = 0;
	char *start = dest;

	while (len < max - 4 && *source)
	{
		ch = (unsigned char)*source;
		if (*source == ' ') {
			*dest++ = '+';
			len++;
		}
		// else if (isalnum(ch) || strchr("-_.!~*'()", ch)) {
		else if (isalnum(ch)) {
			*dest++ = *source;
			len++;
		}
		else {
			*dest++ = '%';
			*dest++ = digits[(ch >> 4) & 0x0F];
			*dest++ = digits[       ch & 0x0F];
			len += 3;
		}  
		source++;
	}
	*dest = 0;
	return start - dest;
}

int main(int argc, char *argv[])
{
	FILE *fp = stdin;
	char input[64 * 1024], output[3 * 64 * 1024], *p;
	int encode = 1, console = 0, loop_per_line = 0;
	int i, n, c;

	if (argc == 1) {
		printf("usage: %s [ -d ] [ -n ] [ -c ]\n", argv[0]);
		exit(1);
	}

	while ((c = getopt(argc, argv, "hdcn")) != -1) {
		switch (c) {
			case 'h':
				printf("usage: %s [ -d ] [ -n ]\n", argv[0]);
				exit(0);
			case 'd':
				encode = 0;
				break;
			case 'c':
				console = 1;
				break;
			case 'n':
				loop_per_line = 1;
				break;
			case '?':
			default:
				exit(1);
		}
	}

	if (console) {
		if (loop_per_line) {
			while (fgets(input, sizeof(input), fp) != NULL) {
				if ((p = strchr(input, '\n')) != NULL)
					*p = '\0';
				if ((p = strchr(input, '\r')) != NULL)
					*p = '\0';
				memset(output, 0, sizeof(output));
				if (encode)
					UrlEncode(input, output, sizeof(output));
				else
					UrlDecode(input, output);
				printf("%s\n", output);
			}
		}
		else {
			memset(input, 0, sizeof(input));
			memset(output, 0, sizeof(output));
			n = fread(input, 1, sizeof(input), fp);

			if (encode) {
				UrlEncode(input, output, sizeof(output));
			}
			else {
				UrlDecode(input, output);
			}
			printf("%s\n", output);
		}
	}
	else {
		for (i = optind; i < argc; i++) {
			memset(output, 0, sizeof(output));
			if (encode)
				UrlEncode(argv[i], output, sizeof(output));
			else
				UrlDecode(argv[i], output);
			printf("%s\n", output);
		}
	}

	return 0;
}
