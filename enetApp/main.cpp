#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

#include <stdexcept>
#include "Client.hpp"

int main() {
	Client client("127.0.0.1", 1234);
	try {
		client.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}