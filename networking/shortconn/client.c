#include "shortconn.h"

static void * client_proc_mt(void * param)
{
	int start_port, port, fd, n;
	char buf[1536];

	start_port = *((int *)param);
	free(param), param = NULL;

	for (;;)
	{
		for (port = start_port; port < start_port + 100; port++)
		{
			if (g.opt_udp) {
				fd = connect_udp_server(g.opt_rhost, g.opt_rport, port);
			}
			else {
				fd = connect_tcp_server(g.opt_rhost, g.opt_rport, port);
			}

			if (fd < 0) {
				if (errno == EADDRNOTAVAIL)
					continue;
				perror("connect_tcp_server");
				goto out;
			}

			if ((n = write(fd, buf, g.opt_packsize)) < 0)
				goto gameover;
			if ((n = read(fd, buf, sizeof(buf))) <= 0)
				goto gameover;

			if ((n = write(fd, buf, g.opt_packsize)) < 0)
				goto gameover;
			if ((n = read(fd, buf, sizeof(buf))) <= 0)
				goto gameover;

			memcpy(buf, "END", 3);
			if ((n = write(fd, buf, g.opt_packsize)) < 0)
				goto gameover;
			if ((n = read(fd, buf, sizeof(buf))) <= 0)
				goto gameover;

		gameover:
			if (n < 0) {
				perror("read/write socket");
				goto out;
			}
			close(fd), fd = -1;

			if (g.opt_oneshot)
				break;
		}

		if (g.opt_oneshot)
			break;
	}

out:
	return NULL;
}

void run_client_mt()
{
	int i;
	pthread_t * tids;

	tids = (pthread_t *)malloc(sizeof(pthread_t) * g.opt_nthread);

	B_INFO("run_client_mt: start...");

	for (i = 0; i < g.opt_nthread; i++) {
		int * start_port = (int *)malloc(sizeof(int));

		*start_port = 10000 + i * 100;

		if (pthread_create(&tids[i], NULL, client_proc_mt, (void *)start_port) < 0) {
			perror("pthread_create");
			break;
		}
	}

	B_INFO("run_client_mt: waiting...");

	for (i = 0; i < g.opt_nthread; i++) {
		pthread_join(tids[i], NULL);
	}

	B_INFO("run_client_mt: done");
}

void run_client()
{
	D_DEBUG("server host: %s", g.opt_rhost);
	D_DEBUG("server port: %d%s", g.opt_rport, g.opt_udp? " (udp)" : "");
	D_DEBUG("concurrent:  %d", g.opt_nthread);
	D_DEBUG("packet size: %d", g.opt_packsize);

	run_client_mt();
}

/*
 vim:ts=4:sw=4:ai:cindent:tw=78:noet
*/
