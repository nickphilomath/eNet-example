#include <iostream>
#include <enet/enet.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

int main2() {
	char buffer[512];
	bool connected = false;

	if (enet_initialize() != 0) {
		std::cout << "cannot initialize enet " << "\n";
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);


	ENetHost* client;
	client = enet_host_create(NULL /* create a client host */,
		1 /* only allow 1 outgoing connection */,
		2 /* allow up 2 channels to be used, 0 and 1 */,
		0 /* assume any amount of incoming bandwidth */,
		0 /* assume any amount of outgoing bandwidth */);
	if (client == NULL)
	{
		fprintf(stderr, "An error occurred while trying to create an ENet client host.\n");
		exit(EXIT_FAILURE);
	}

	ENetAddress address;
	ENetEvent event;
	ENetPeer* peer;
	/* Connect to some.server.net:1234. */
	enet_address_set_host(&address, "127.0.0.1");
	address.port = 1234;

	peer = enet_host_connect(client, &address, 2, 0);
	if (peer == NULL)
	{
		fprintf(stderr, "No available peers for initiating an ENet connection.\n");
		exit(EXIT_FAILURE);
	}
	/* Wait up to 5 seconds for the connection attempt to succeed. */
	if (enet_host_service(client, &event, 3000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
	{
		puts("Connection succeeded.");
		connected = true;

		// send a packet 
		ENetPacket* packet = enet_packet_create("hello packet", strlen("hello packet") + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
	}
	else {
		enet_peer_reset(peer);
		puts("Connection failed.");
		return EXIT_FAILURE;
	}

	while (true) {
		while (enet_host_service(client, &event, 1000) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_RECEIVE:
				std::cout
					<< "A packet of length" << event.packet->dataLength
					<< "containing" << event.packet->data
					<< "was received from " << event.peer->data
					<< " on channel " << event.channelID
					<< ".\n";
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				connected = false;
				std::cout << event.peer->data << " disconnected. \n";
				/* Reset the peer's client information. */
				event.peer->data = NULL;
				return 0;
			}
		}
		if (connected) {
			std::cout << "enter text: \n"; std::cin >> buffer;
			ENetPacket* packet = enet_packet_create(buffer, strlen(buffer) + 1, ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(peer, 0, packet);
		}
	}

	enet_host_destroy(client);

	return EXIT_SUCCESS;
}