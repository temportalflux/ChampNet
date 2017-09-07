#include "Network\Client.h"

Client::Client(Network::PacketHandler* packetHandler) : Framework(false, packetHandler) {

}

Client::~Client() {
}

void Client::prompt() {
	Framework::prompt();

	char str[512];

	// Get the IP to connect to
	printf("Enter server IP or hit enter for 127.0.0.1... ");
	fgets(str, 512, stdin);
	if (str[0] == '\n') {
		strcpy(str, "127.0.0.1");
	}
	this->mAddressServerStr = std::string(str);

}

Network::Network* Client::createNetwork() {
	// Create and return the appropriate network object
	return new Network::NetworkClient(this->mAddressServerStr, this->mPort);
}
