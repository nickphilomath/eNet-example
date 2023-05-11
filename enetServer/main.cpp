#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

#include <iostream>
#include <stdexcept>
#include <enet/enet.h>

constexpr auto PORT = 1234;

#include "Server.hpp"

int main() {
	Server server(PORT);
	try {
		server.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}