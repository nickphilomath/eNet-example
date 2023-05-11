#include "Server.hpp"

Server::Server(uint32_t port) {
	address.host = ENET_HOST_ANY;
	address.port = port;

	if (enet_initialize() != 0) {
		throw std::runtime_error("cannot initialize enet");
	}

	serverHost = enet_host_create(&address,
		32,   //number of clients
		2,    //number of channels
		0,    // assume any amount of incoming bandwidth 
		0     // assume any amount of outgoing bandwidth
	);

}

Server::~Server() {
	enet_host_destroy(serverHost);
	enet_deinitialize();
}

void Server::Initialize() {
	
}

void Server::run() {

	if (serverHost == NULL)
	{
		throw std::runtime_error("An error occurred while trying to create an ENet server host");
	}

	while (true) {
		listen();
	}
}

void Server::listen() {
	while (enet_host_service(serverHost, &event, 1000) > 0) {
		switch (event.type) {
		case ENET_EVENT_TYPE_CONNECT:
			peers.push_back(event.peer);
			std::cout
				<< "A new client connected from " << event.peer->address.host
				<< " port: " << event.peer->address.port
				<< " peerID: " << event.peer->incomingPeerID
				<< ".\n";
			/* Store any relevant client information here. */
			//event.peer->data = "Client information";
			break;

		case ENET_EVENT_TYPE_RECEIVE:
			std::cout
				<< "peer " << event.peer->incomingPeerID
				<< " says: '" << event.packet->data
				<< "' on channel " << event.channelID
				<< ".\n";
			/* Clean up the packet now that we're done using it. */
			enet_packet_destroy(event.packet);
			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			std::cout << event.peer->incomingPeerID << " disconnected. \n";
			// remove from peers array
			for (uint32_t i = 0; i < peers.size(); i++) {
				if (peers[i]->incomingPeerID == event.peer->incomingPeerID) {
					peers.erase(peers.begin() + i);
				}
			}
			// std::cout << "// vector size: " << peers.size() << "\n";
			/* Reset the peer's client information. */
			event.peer->data = NULL;
		}
	}
}

