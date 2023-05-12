#pragma once

#include <stdint.h>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <enet/enet.h>

class Server {
public:
	Server(uint32_t port);
	~Server();

	void run();

private:
	void Initialize();
	void listen();

	void send(const std::string& buffer, ENetPeer* peer);

	ENetAddress address;
	ENetHost* serverHost;
	ENetEvent event;
	std::vector<ENetPeer*> peers;
};