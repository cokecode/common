/*
 * TCP/UDP shortconn performance test utility.
 * Paul Chen <yongbao.chen@net263.com>
 * 2006.7
 */

#include "shortconn.h"

/* ------------------------------------------------------------------------ */

#define VERSION_MAJOR		0
#define VERSION_MINOR		0
#define VERSION_PATCH		2

struct shortconn_g g = {
	.opt_listen = 0,
	.opt_port = 6889,
	.opt_nthread = 1,
	.opt_packsize = 512,
};

/* ------------------------------------------------------------------------ */

static void usage()
{
	printf(
	"TCP/shortconn utitily %d.%d.%d\n"
	"Copyright by yongbao.chen@net263.com, 2006.7\n"
	"Usage:\n"
	"   shortconn --help\n"
	"   shortconn [ options ]\n"
	"Options:\n"
	"   -h --help           print this message\n"
	"   -v --version        print this message\n"
	"   -L --listen         run as server\n"
	"   -u --udp            use udp instead of tcp\n"
	"   -p --port ##        source port\n"
	"   -s --packsize ##    packet size (default 512bytes)\n"
	"   -t --nthread ##     # concurrent thread\n"
	"   -1 --oneshot        oneshot connection\n"
	"   -e --emucalc        emulate app calculation (server only)\n"
	"",
	VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH
	);
}

static void parse_cmdline(int argc, char * argv[])
{
	struct option opts[] = {
		{ "help",		0, NULL, 'h', },
		{ "version",	1, NULL, 'v', },
		{ "listen",		0, NULL, 'L', },
		{ "udp",		0, NULL, 'u', },
		{ "port",		1, NULL, 'p', },
		{ "packsize",	1, NULL, 's', },
		{ "nthread",	1, NULL, 't', },
		{ "oneshot",    0, NULL, '1', },
		{ "emucalc",    0, NULL, 'e', },
		{ NULL,			0, NULL, 0, },
	};
	char c;

	while ((c = getopt_long(argc, argv, "hvLt:p:s:u1e", opts, NULL)) != -1)
	{
		switch (c)
		{
			case 'h':
			case 'v':
				usage();
				exit(1);
				break;
			case 'L':
				g.opt_listen = 1;
				break;
			case 'u':
				g.opt_udp = 1;
				break;
			case 'p':
				g.opt_port = atoi(optarg);
				break;
			case 's':
				g.opt_packsize = atoi(optarg);
				break;
			case 't':
				g.opt_nthread = atoi(optarg);
				break;
			case '1':
				g.opt_oneshot = 1;
				break;
			case 'e':
				g.opt_emucalc = 1;
				break;
			default:
				// getopt_long() has printed some error message.
				exit(1);
				break;
		}
	}

	if (++optind <= argc)
		g.opt_rhost = argv[optind - 1];
	if (++optind <= argc)
		g.opt_rport = atoi(argv[optind - 1]);
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
		case SIGTERM:		/* kill default */
			break;
		case SIGINT:		/* Ctrl-C */
			close(g.g_servsock), g.g_servsock = -1;
			g.f_barrier_start = 1;
			break;
		case SIGQUIT:		/* Ctrl-\ */
			g.f_barrier_start = 1;
			break;
		case SIGUSR1:
		case SIGUSR2:
		case SIGHUP:
			break;
		default:
			break;
	}		
}

int main(int argc, char * argv[])
{
	parse_cmdline(argc, argv);

	if (g.opt_listen) {
		my_signal(SIGINT, sighdl);
		my_signal(SIGQUIT, sighdl);
		run_server();
	}
	else {
		run_client();
	}

	return 0;
}

/*
 vim:ts=4:sw=4:ai:cindent:tw=78:noet
*/
