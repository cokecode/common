#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <enet/enet.h>
#include "simplelog.h"


int main()
{
	ENetAddress addr;
	ENetHost * server = NULL;
	ENetEvent event;
	int retval;

	if (enet_initialize() != 0) {
		D_ERROR("enet_initialize() failed");
		exit(1);
	}

	atexit(enet_deinitialize);

	addr.host = ENET_HOST_ANY;
	addr.port = 6889;
	server = enet_host_create(&addr, 128, 0, 0);
	if (server == NULL) {
		D_ERROR("enet_create_host() failed");
		exit(1);
	}

	D_INFO("created server");

	//
	memset(&event, 0, sizeof(event));
	for (;;)
	{
		// D_DEBUG("listen...");
		retval = enet_host_service(server, &event, 1000000);
		if (retval < 0) {
			D_ERROR("enet_host_service() failed");
			break;
		}
		if (retval == 0)
			continue;

		// D_DEBUG("here..........");
		switch (event.type)
		{
			case ENET_EVENT_TYPE_CONNECT:
				D_INFO("New client connected: %x:%d",
						event.peer->address.host,
						event.peer->address.port);
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				/*
				D_INFO("A packet of length %u containing %s was received "
						"from %s on channel %u.",
						event.packet->dataLength,
						event.packet->data,
						(char *)event.peer -> data,
						event.channelID);
				*/
				printf("[%x:%d:%d] %s",
						event.peer->address.host,
						event.peer->address.port,
						event.channelID,
						event.packet->data);
				enet_peer_send(event.peer, event.channelID, event.packet);
				// enet_packet_destroy(event.packet), event.packet = NULL;
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				D_INFO("%x:%d disconected.", event.peer->address.host,
						event.peer->address.port);
				enet_peer_disconnect(event.peer);
				event.peer->data = NULL;
				break;
			default:
				D_WARN("Unknown event type '%d'", event.type);
				break;
		}
	}

	D_INFO("server loop ended");

	enet_host_destroy(server), server = NULL;

	exit(0);
}
