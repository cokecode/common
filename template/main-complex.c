
static void usage()
{
	printf(
	""
	);
}

static void parse_cmdline(int argc, char * argv[])
{
	struct option opts[] = {
		{ "help", 0, NULL, 'h', },
		{ "version", 2, NULL, 'v', },
		{ NULL, 0, NULL, 0, },
	};
	char c;

	while ((c = getopt_long(argc, argv, "hv", opts, NULL)) != -1)
	{
		switch (c)
		{
			case 'h':
			case 'v':
				usage();
				exit(1);
				break;
			default:
				// getopt_long() has printed some error message.
				exit(1);
				break;
		}
	}

	if (++optind <= argc)
		argv[optind - 1];
	if (++optind <= argc)
		argv[optind - 1];
	if (++optind <= argc)
		argv[optind - 1];
}

static void my_signal(int signum, __sighandler_t handler)
{   
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handler;
	sa.sa_flags = SA_RESTART;

	if (sigaction(signum, &sa, NULL) < 0) {
		perror("sigaction");
		exit(1);
	}
}

static void sighdl(int signum)
{
	switch (signum)
	{
		case SIGINT:
		case SIGTERM:
		case SIGQUIT:
		case SIGUSR1:
		case SIGUSR2:
		case SIGHUP:
		case SIGWINCH:
			break;
		default:
			break;
	}       
}

int main(int argc, char * argv[])
{
	parse_cmdline(argc, argv);

	return 0;
}
