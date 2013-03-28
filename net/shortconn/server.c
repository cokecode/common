#include "shortconn.h"

/* ------------------------------------------------------------------------ */
/* Miscellaneous routines */

static void do_calc1()
{
	int size = 256;
	char * p = (char *)malloc(size);
	time_t a, b;
	int i;

	time(&a);

	for (i = 0; i < size; i++) {
		p[i] = i * i * i;
	}

	memset(p, 0, size);
	free(p), p = NULL;

	time(&b);
}

static void do_calc2()
{
	do_calc1();
}

/* ------------------------------------------------------------------------ */
/* UDP server */

static void run_udp_server()
{
	char buf[1536];
	int sfd, n;
	struct sockaddr sa;
	socklen_t salen = sizeof(sa);

	if ((sfd = create_udp_server(NULL, g.opt_port)) < 0)
		return;

	g.g_servsock = sfd;

	B_INFO("Server listening on udp port %d", g.opt_port);
	B_INFO("Press Ctrl-\\ to start.");

	while (g.f_barrier_start == 0)
		sleep(1);

	B_INFO("Flood start! press Ctrl-C to stop.");
	g.st_time_begin = getsystime();

	for (;;) {
		n = recvfrom(sfd, buf, sizeof(buf), 0, &sa, (socklen_t *)&salen);
		if (n < 0) {
			perror("recvfrom");
			break;
		}

		if (g.opt_emucalc)
			do_calc1();

		if (sendto(sfd, buf, n, 0, &sa, salen) < 0) {
			perror("sendto");
			break;
		}

		if (memcmp(buf, "END", 3) == 0)
			atomic_inc(&g.st_nsession);

		if (g.opt_emucalc)
			do_calc2();
	}

	g.st_time_end = getsystime();
	B_INFO("Stopped.");
}

/* ------------------------------------------------------------------------ */
/* TCP server */

static void * server_proc_mt(void * param)
{
	char buf[1536];
	int fd, n;

	fd = *((int *)param);
	free(param), param = NULL;

	if (g.opt_emucalc)
		do_calc1();

	/* step1 */
	if ((n = read(fd, buf, sizeof(buf))) <= 0)
		goto out;
	if ((n = write(fd, buf, n)) < 0)
		goto out;

	/* step2 */
	if ((n = read(fd, buf, sizeof(buf))) <= 0)
		goto out;
	if ((n = write(fd, buf, n)) < 0)
		goto out;

	/* step3 */
	if ((n = read(fd, buf, sizeof(buf))) <= 0)
		goto out;
	if ((n = write(fd, buf, n)) < 0)
		goto out;

	/* step4: wait for client to close the socket */
	{
		fd_set fds;
		struct timeval timeout = { 1, 0 };

		FD_ZERO(&fds);
		FD_SET(fd, &fds);

		/* failure is ok: the socket will be closed any way */
		select(fd + 1, &fds, NULL, NULL, &timeout);
	}

out:
	if (n < 0) {
		perror("server_proc_mt");
		atomic_inc(&g.st_nsession_bad);
	}
	else {
		atomic_inc(&g.st_nsession);
	}

	if (g.opt_emucalc)
		do_calc1();

	close(fd), fd = -1;		/* server close the socket */
	return NULL;
}

static void run_tcp_server_mt()
{
	int servsock, clntsock;

	if ((servsock = create_tcp_server(NULL, g.opt_port)) < 0)
		return;

	g.g_servsock = servsock;

	B_INFO("Server listening on port %d ...", g.opt_port);
	B_INFO("Press Ctrl-\\ to start.");

	while (g.f_barrier_start == 0)
		sleep(1);

	B_INFO("Flood start! press Ctrl-C to stop.");
	g.st_time_begin = getsystime();

	while ((clntsock = accept(servsock, NULL, NULL)) != -1) {
		int * param = (int *)malloc(sizeof(int));
		pthread_attr_t attr;
		pthread_t tid;

		*param = clntsock;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

		if (pthread_create(&tid, &attr, server_proc_mt, (void *)param) < 0) {
			free(param), param = NULL;
			perror("pthread_create");
			break;
		}
	}

	g.st_time_end = getsystime();
	B_INFO("Stopped.");
}

void print_stat()
{
	double delta, avg;

	delta = g.st_time_end - g.st_time_begin;
	avg = delta == 0? 0 : atomic_read(&g.st_nsession) / delta * 1000;

	printf("delta = %.2f\n", delta);
	printf("count = %d\n", atomic_read(&g.st_nsession));
	printf("avg = %.2f\n", avg);
}

/* ------------------------------------------------------------------------ */
/* Server runs here */

void run_server()
{
	D_DEBUG("server port: %d", g.opt_port);

	if (g.opt_udp)
		run_udp_server();
	else
		run_tcp_server_mt();

	print_stat();
}

/*
 vim:ts=4:sw=4:ai:cindent:tw=78:noet
*/
