#include <enet/enet.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

int main3() {
	std::cout << "server started" << std::endl;

	if (enet_initialize() != 0) {
		std::cout << "cannot initialize enet " << "\n";
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	ENetAddress address;
	ENetHost* server;
	ENetEvent event;
	/* Bind the server to the default localhost.     */
	/* A specific host address can be specified by   */
	/* enet_address_set_host (& address, "x.x.x.x"); */
	address.host = ENET_HOST_ANY;
	address.port = 1234;
	server = enet_host_create(&address /* the address to bind the server host to */,
		32      /* allow up to 32 clients and/or outgoing connections */,
		2      /* allow up to 2 channels to be used, 0 and 1 */,
		0      /* assume any amount of incoming bandwidth */,
		0      /* assume any amount of outgoing bandwidth */);
	if (server == NULL)
	{
		fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
		exit(EXIT_FAILURE);
	}

	while (true) {
		/* Wait up to 1000 milliseconds for an event. */
		while (enet_host_service(server, &event, 1000) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				std::cout
					<< "A new client connected from " << event.peer->address.host
					<< " port: " << event.peer->address.port
					<< ".\n";
				/* Store any relevant client information here. */
				//event.peer->data = "Client information";
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				std::cout
					<< "A packet of length " << event.packet->dataLength
					<< " containing " << event.packet->data
					<< " was received from " << event.peer->data
					<< " on channel " << event.channelID
					<< " incoming peer id " << event.peer->incomingPeerID
					<< " outgoing peer id " << event.peer->outgoingPeerID
					<< ".\n";
				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy(event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				std::cout << event.peer->data << " disconnected. \n";
				/* Reset the peer's client information. */
				event.peer->data = NULL;
			}
		}

	}

	enet_host_destroy(server);
}