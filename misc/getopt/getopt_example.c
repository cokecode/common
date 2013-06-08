#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

struct {
    int daemon;
    char cfgfile[1024];
} g_opts;

static void print_usage()
{
    printf("help message ...\n");
}

static void print_version()
{
    printf("version ...\n");
}

static void parse_cmdline(int argc, char * argv[])
{
	struct option opts[] =
	{
		{ "help", 0, NULL, 'h' },
		{ "version", 0, NULL, 'v' },
		{ "foreground", 0, NULL, 'x' },
		{ "terminate", 0, NULL, 't' },
		{ "config", 1, NULL, 'f' },
        { "long-only1", 1, NULL, 0 },
        { "long-only2", 0, NULL, 0 },
		{ NULL, 0, NULL, 0, },
	};
	char c;
    int longidx = -1;

	while ((c = getopt_long(argc, argv, "hvtxf:", opts, &longidx)) != -1)
	{
		switch (c)
		{
			case 'h':
				print_usage();
				exit(1);
			case 'v':
				print_version();
				exit(1);
			case 'x':
				g_opts.daemon = 0;
				break;
			case 't':
				printf("ignore -t...\n");
				exit(0);
			case 'f':
				strncpy(g_opts.cfgfile, optarg, sizeof(g_opts.cfgfile) - 1);
				break;
			case '?':
			default:
                printf("longidx = %d\n", longidx);
                if (longidx != -1) {
                    printf("got option %s\n", opts[longidx].name);
                }
                else {
                    // getopt_long() has printed some error message.
                    exit(1);
                }
		}
	}
}

int main(int argc, char *argv[])
{
    parse_cmdline(argc, argv);
    return 0;
}

