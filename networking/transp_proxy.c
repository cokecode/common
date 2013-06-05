/*
 * transp_proxy.c
 *
 * transparent proxy
 *
 * YuLei <yulei@onewaveinc.com>
 *
 * */

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <syslog.h>

#define RELEASE_VERSION "release_0_5_0"
#define PROG_NAME "transp_proxy"
static const char *rcsid __attribute__ ((unused)) = "$Id: transp_proxy.c,v 1.4 2006-05-03 03:07:37 uray Exp $";

static int g_drop_packet = 0;

static void
err (const char *fmt, ...)
{
  struct stat stat_buf;
  va_list ap;

  va_start (ap, fmt);

  if (fstat (fileno (stderr), &stat_buf) == 0)
  {
    vfprintf (stderr, fmt, ap);
    fflush (stderr);
  }
  else
    vsyslog (LOG_DAEMON | LOG_ERR, fmt, ap);

  va_end (ap);
}

void
handle_tcp_data (int upstream_socket, int downstream_socket)
{
  while (1)
  {
    fd_set read_fds;
    int ret;

    char buf[65536];
    ssize_t data_size;

    FD_ZERO (&read_fds);
    FD_SET (downstream_socket, &read_fds);
    FD_SET (upstream_socket, &read_fds);

    ret = select (upstream_socket + 1, &read_fds, NULL, NULL, NULL);
    if (ret < 0)
    {
      if (errno == EINTR)
	continue;
      err ("select fail: %s\n", strerror (errno));
      break;
    }
    if (ret == 0)
      break;

    if (FD_ISSET (downstream_socket, &read_fds))
    {
      data_size = read (downstream_socket, buf, sizeof(buf));
      if (data_size <= 0)
	break;

      write (upstream_socket, buf, data_size);
    }

    if (FD_ISSET (upstream_socket, &read_fds))
    {
      data_size = read (upstream_socket, buf, sizeof(buf));
      if (data_size <= 0)
	break;

      if (g_drop_packet && random() % 2 == 0)
	printf("Drop %d bytes\n", data_size);
      else
	write (downstream_socket, buf, data_size);
    }
  } // while (1);

  close (downstream_socket);
  close (upstream_socket);
}

void
handle_tcp_connection (struct sockaddr_in *listen_sockaddr,
    struct sockaddr_in *upstream_sockaddr)
{
  int listen_socket;
  int conn_count = 0;
  int v;

  listen_socket = socket (PF_INET, SOCK_STREAM, 0);
  if (listen_socket == -1)
  {
    err ("create socket for listen fail: %s\n", strerror (errno));
    return;
  }

  v = 1;
  if (setsockopt (listen_socket, SOL_SOCKET, SO_REUSEADDR, &v,sizeof (v)) == -1)
  {
    err ("setsockopt fail: %s\n", strerror (errno));
    return;
  }

  if (bind (listen_socket, (struct sockaddr*)listen_sockaddr,
	sizeof (struct sockaddr_in)) == -1)
  {
    err ("bind fail: %s\n", strerror (errno));
    return;
  }

  if (listen (listen_socket, 5) == -1)
  {
    err ("listen fail: %s\n", strerror (errno));
    return;
  }

  while (1)
  {
    struct sockaddr_in downstream_sockaddr;
    int downstream_socket;
    int upstream_socket;
    socklen_t addrlen;

    addrlen = sizeof (struct sockaddr_in);
    memset (&downstream_sockaddr, 0, sizeof (struct sockaddr_in));
    downstream_socket =
      accept (listen_socket, (struct sockaddr*)&downstream_sockaddr, &addrlen);
    if (downstream_socket == -1)
    {
      err ("accept fail: %s\n", strerror (errno));
      continue;
    }

    conn_count++;
    printf ("[%04d  new connection from %s:%d\n", conn_count,
	inet_ntoa (downstream_sockaddr.sin_addr),
	ntohs (downstream_sockaddr.sin_port));

    upstream_socket = socket (PF_INET, SOCK_STREAM, 0);
    if (upstream_socket == -1)
    {
      err ("create socket for upstream fail: %s\n", strerror (errno));
      close (downstream_socket);
      continue;
    }

    printf ("	connecting to %s:%d ... ", 
	inet_ntoa (upstream_sockaddr->sin_addr),
	ntohs (upstream_sockaddr->sin_port));
    fflush (stdout);
    if (connect (upstream_socket, (struct sockaddr*)upstream_sockaddr,
	  sizeof (struct sockaddr)) == -1)
    {
      printf ("fail: %s\n", strerror (errno));
      close (upstream_socket);
      close (downstream_socket);
      continue;
    }
    printf ("success\n");

    if (fork () != 0)
    {
      while (waitpid (-1, NULL, WNOHANG) > 0);
      close (downstream_socket);
      close (upstream_socket);
      continue;
    }

    handle_tcp_data (upstream_socket, downstream_socket);

    printf (" %04d] connection from %s:%d closed\n", conn_count,
	inet_ntoa (downstream_sockaddr.sin_addr),
	ntohs (downstream_sockaddr.sin_port));

    break;
  } // while (1)
}

void
handle_udp_data (struct sockaddr_in *listen_sockaddr,
    struct sockaddr_in *upstream_sockaddr)
{
  int listen_socket;
  int upstream_socket;

  listen_socket = socket (PF_INET, SOCK_DGRAM, 0);
  if (listen_socket == -1)
  {
    err ("create socket fail: %s\n", strerror (errno));
    return;
  }

  if (bind (listen_socket, (struct sockaddr*)listen_sockaddr,
	sizeof (struct sockaddr_in)) == -1)
  {
    err ("bind fail: %s\n", strerror (errno));
    return;
  }

  upstream_socket = socket (PF_INET, SOCK_DGRAM, 0);
  if (upstream_socket == -1)
  {
    err ("create socket for upstream fail: %s\n", strerror (errno));
    close (listen_socket);
    return;
  }

  if (connect (upstream_socket, (struct sockaddr*)upstream_sockaddr,
	sizeof (struct sockaddr)) == -1)
  {
    printf ("connect fail: %s\n", strerror (errno));
    close (listen_socket);
    close (upstream_socket);
    return;
  }

  while (1)
  {
    char buf[65536];
    ssize_t data_size;

    data_size = recv (listen_socket, buf, sizeof (buf), 0);
    if (data_size > 0)
    {
      ssize_t ret;
      ret = send (upstream_socket, buf, data_size, 0);
      if (ret != data_size)
	perror ("send fail");
    }
    else
    {
      perror ("recv fail\n");
      sleep (1);
    }
  }
}

void
print_usage ()
{
  printf ("Usage: %s <OPTIONS> ...\n", PROG_NAME);
  printf ("  -a, --listen_addr=ADDR     listen address\n");
  printf ("  -p, --listen_port=PORT     listen port\n");
  printf ("  -A, --upstream_addr=ADDR   upstream address\n");
  printf ("  -P, --upstream_port=PORT   upstream port\n");
  printf ("  -D, --daemon               enter daemon mode\n");
  printf ("  -u, --udp                  retransmit udp packet from listen_addr to upstream\n");
  printf ("    -h, --help               print this help text and exit\n");
  printf ("    -v, --version            output version information and exit\n");
  printf ("\nReport bugs to <yulei@onewaveinc.com>\n");
}

void
sighdl(int signum)
{
    g_drop_packet = !g_drop_packet;

    if (g_drop_packet)
	printf("Switch to drop-packet status\n");
    else
	printf("Switch to normal status\n");
}

int
main (int argc, char* argv[])
{
  struct sockaddr_in upstream_sockaddr;
  struct sockaddr_in listen_sockaddr;
  int daemon_mode = 0;
  int udp_mode = 0;

  memset (&upstream_sockaddr, 0, sizeof (struct sockaddr_in));
  memset (&listen_sockaddr, 0, sizeof (struct sockaddr_in));

  upstream_sockaddr.sin_family = AF_INET;
  upstream_sockaddr.sin_addr.s_addr = INADDR_ANY;
  listen_sockaddr.sin_family = AF_INET;
  listen_sockaddr.sin_addr.s_addr = INADDR_ANY;

  signal(SIGQUIT, sighdl);

  while (1)
  {
    int c;
    int option_index = 0;
    static struct option long_opts[] =
    {
      { "listen_addr", 1, 0, 'a' },
      { "listen_port", 1, 0, 'p' },
      { "upstream_addr", 1, 0, 'A' },
      { "upstream_port", 1, 0, 'P' },
      { "udp", 0, 0, 'u' },
      { "daemon", 0, 0, 'D' },
      { "help", 0, 0, 'h' },
      { "version", 0, 0, 'v' },
      { NULL, 0, 0, 0 }
    };

    c = getopt_long (argc, argv, "hvDua:p:A:P:", long_opts, &option_index);
    if (c == -1)
      break;

    switch (c)
    {
      case 'h':
	print_usage ();
	return 1;
      case 'v':
        printf ("%s %s\n", PROG_NAME, RELEASE_VERSION);
        printf ("Written by YuLei\n");
	return 1;
      case 'D':
	daemon_mode = 1;
	break;
      case 'a':
	if (inet_pton (AF_INET, optarg, &listen_sockaddr.sin_addr) <= 0)
	{
	  err ("invalid list_addr: %s\n", strerror (errno));
	  return -1;
	}
	break;
      case 'p':
	listen_sockaddr.sin_port = htons (atoi (optarg));
	if (listen_sockaddr.sin_port <= 0)
	{
	  err ("invalid listen_port\n");
	  return -1;
	}
	break;
      case 'A':
	if (inet_pton (AF_INET, optarg, &upstream_sockaddr.sin_addr) == -1)
	{
	  err ("invalid upstream_addr: %s\n", strerror (errno));
	  return -1;
	}
	break;
      case 'P':
	upstream_sockaddr.sin_port = htons (atoi (optarg));
	if (upstream_sockaddr.sin_port <= 0)
	{
	  err ("invalid upstream_port\n");
	  return -1;
	}
	break;
      case 'u':
	udp_mode = 1;
	break;
      default:
	print_usage ();
	return -1;
    }
  }
  if (optind < argc)
  {
    err ("Unknown arguments: ");
    while (optind < argc)
      err (argv[optind++]);
    err ("\n");

    print_usage ();
    return -1;
  }

  if (upstream_sockaddr.sin_addr.s_addr == INADDR_ANY)
  {
    err ("missing argument 'upstream_addr'\n\n");
    print_usage ();
    return -1;
  }
  if (upstream_sockaddr.sin_port == 0)
  {
    err ("missing argument 'upstream_port'\n\n");
    print_usage ();
    return -1;
  }
  if (listen_sockaddr.sin_port == 0)
  {
    err ("missing argument 'listen_port'\n\n");
    print_usage ();
    return -1;
  }

  signal (SIGPIPE, SIG_IGN);

  if (daemon_mode)
    daemon (0, 0);

  printf ("Listen on %s:%d ...\n",
      inet_ntoa (listen_sockaddr.sin_addr), ntohs (listen_sockaddr.sin_port));

  if (udp_mode)
    handle_udp_data (&listen_sockaddr, &upstream_sockaddr);
  else
    handle_tcp_connection (&listen_sockaddr, &upstream_sockaddr);

  return 0;
}
