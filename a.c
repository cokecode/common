#define INTEL_HEX_RECORD_MINSIZE	14

int send_hex_file(const char * filename)
{
	const char * filename = "aaa.txt";
	char buf[1024];
	const char * p;
	FILE * fp;
	int record_length;
	int record_type;
	int target_addr;
	const char * record_data;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("failed to open file [%s]\n", filename);
		exit(1);
	}

	/*
	 * record format:
	 *   :llaaaatt[dd...]cc
	 */

	while ((n = fread(buf, 1, sizeof(buf), fp)) > 0) {
		if (n < INTEL_HEX_RECORD_MINSIZE) {
			printf("record not complete\n");
			break;
		}
		if (buf[0] != ':') {
			printf("invalid record\n");
			break;
		}

		p = buf + 1;
		record_length = hex2int8(p);
		p += 2;
		target_addr = hex2int16(p);
		p += 4;
		record_type = hex2int8(p);
		p += 2;
		record_data = p;

		if (!check_chksum(buf, 1 + 2 + 4 + 2 * record_length)) {
			printf("incorrect checksum\n");
			break;
		}

		// TODO parse record_type, calculate addr, send data ...
	}

	fclose(fp);
	fp = NULL;
}

int hex2int8(const char * p)
{
	char buf[256];
	snprintf(buf, sizeof(buf), "0x%c%c", p[0], p[1]);
	return strtol(buf, NULL, 0);
}

int hex2int16(const char * p)
{
	char buf[256];
	snprintf(buf, sizeof(buf), "0x%c%c%c%c", p[0], p[1], p[2], p[3]);
	return strtol(buf, NULL, 0);
}

int check_chksum(const char * record, int len)
{
	int chksum = 0;
	const char * p = record + 1;	// skip leading ':'

	// according to the "hexformat.pdf" document
	chksum += 0x01;

	for (i = 0; i < len; ) {
		chksum += hex2int8(p + i);
		i += 2;
	}

	chksum = (~chksum + 0x01) % 256;

	return (chksum == hex2int8(record + len));
}

