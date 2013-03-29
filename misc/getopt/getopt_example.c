static void parse_cmdline(int argc, char * argv[])
{
	struct option opts[] =
	{
		{ "--help", 0, NULL, 'h', },
		{ "--version", 0, NULL, 'v', },
		{ "--foreground", 0, NULL, 'x', },
		{ "--terminate", 0, NULL, 't', },
		{ "--config", 1, NULL, 'f', },
		{ NULL, 0, NULL, 0, },
	};
	char c;

	while ((c = getopt_long(argc, argv, "hvtxf:", opts, NULL)) != -1)
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
				g_opts.daemon = false;
				break;
			case 't':
				printf("ignore -t...\n");
				exit(0);
			case 'f':
				strncpy(g_opts.cfgfile, optarg, sizeof(g_opts.cfgfile) - 1);
				break;
			case '?':
			default:
				// getopt_long() has printed some error message.
				exit(1);
		}
	}
}
