#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <enet/enet.h>
#include "simplelog.h"

static int g_exit_flag = 0;

void sighdl(int signum);
void proc_echo(ENetHost * client, ENetPeer * peer);

// Usage:
//   $1 <servaddr> <servport>
int main(int argc, char * argv[])
{
	const char * servaddr = NULL;	// servaddr must be specified
	uint16_t servport = 6889;		// default server port
	ENetAddress addr;
	ENetHost * client = NULL;
	ENetPeer * peer = NULL;
	ENetEvent event;

	// parse command line
	if (argc == 1) {
		fprintf(stderr, "Usage: $1 <servaddr> <servport>\n");
		exit(1);
	}

	if (argc > 1)
		servaddr = argv[1];
	if (argc > 2)
		servport = atoi(argv[2]);

	// enet initialize & create host object
	if (enet_initialize() != 0) {
		D_ERROR("enet_initialize() failed");
	}
	atexit(enet_deinitialize);

	client = enet_host_create(NULL, 1, 0, 0);
	if (client == NULL) {
		D_ERROR("enet_host_create() failed");
		exit(1);
	}

	// connect
	enet_address_set_host(&addr, servaddr);
	addr.port = servport;

	peer = enet_host_connect(client, &addr, 2);
	if (peer == NULL) {
		D_ERROR("No available peers for initializing an ENet connection");
		exit(1);
	}

	if (enet_host_service(client, &event, 5000) > 0
		&& event.type == ENET_EVENT_TYPE_CONNECT)
	{
		D_INFO("connected to server");
	}
	else {
		enet_peer_reset(peer), peer = NULL;
		D_ERROR("failed to connect to server");
		exit(1);
	}

	sleep(1);

	// send packet
	/*
	packet = enet_packet_create(message, strlen(message) + 1,
			ENET_PACKET_FLAG_RELIABLE);
	enet_packet_resize(packet, strlen(message2) + 1);
	strcpy(packet->data, message2);
	enet_peer_send(peer, 0, packet);
	enet_host_flush(client);
	D_INFO("sent packet");
	*/
	proc_echo(client, peer);

	sleep(1);

	// disconnect
	enet_peer_disconnect(peer);
	while (enet_host_service(client, &event, 3000) > 0)
	{
		switch (event.type)
		{
			case ENET_EVENT_TYPE_RECEIVE:
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				D_INFO("diconnected successfully");
				break;
			default:
				D_WARN("unknown event type %d", event.type);
				break;
		}
	}

	// end
	D_INFO("destroying ...");

	enet_peer_reset(peer), peer = NULL;
	
	enet_host_destroy(client), client = NULL;

	return 0;
}

void sighdl(int signum)
{
	switch (signum)
	{
		case SIGINT:
			g_exit_flag = 1;
			break;
		default:
			break;
	}
}

void proc_echo(ENetHost * client, ENetPeer * peer)
{
	ENetPacket * packet = NULL;
	ENetEvent event;
	char buf[1024];
	int connected = 1;
	int retval;

	while (connected && !g_exit_flag) {
		memset(buf, 0, sizeof(buf));
		fgets(buf, sizeof(buf), stdin);

		if (strncmp(buf, "exit", 4) == 0) {
			enet_peer_disconnect(peer);
		}
		else {
			packet = enet_packet_create(buf, strlen(buf) + 1,
				ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(peer, 0, packet);
			// destroy packet??
		}
		
		retval = enet_host_service(client, &event, 5000);
		if (retval < 0) {
			D_ERROR("enet_host_service() failed");
			break;
		}
		if (retval == 0) {
			D_ERROR("enet_host_service() timeout expired");
		}

		switch (event.type)
		{
			case ENET_EVENT_TYPE_CONNECT:
				D_WARN("SHIT!!");
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				connected = 0;
				D_INFO("disconnected");
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				printf("%x:%d> %s", event.peer->address.host,
						event.peer->address.port, event.packet->data);
				enet_packet_destroy(event.packet), event.packet = NULL;
				break;
			default:
				D_INFO("event type = %d", event.type);
				break;
		}
	}
}
