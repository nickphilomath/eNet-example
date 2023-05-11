#pragma once

#include <iostream>
#include <stdexcept>
#include <enet/enet.h>

class Client {
public:
	Client(std::string addressName, uint32_t portNum);
	~Client();

	void run();

private:
	void connect();
	void send(const std::string& buffer);
	void recieve();
	void disconnect();

	bool connected = false;
	std::string serverAddress;
	ENetHost* clientHost;
	ENetAddress address;
	ENetEvent event;
	ENetPeer* peer;
};