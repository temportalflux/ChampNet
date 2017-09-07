#include "Network\Server.h"

#include <iostream>

Server::Server(Network::PacketHandler* packetHandler) : Framework(true, packetHandler) {

}

Server::~Server() {
}

void Server::prompt() {
	Framework::prompt();

	char str[512];

	// Get max clients if creating a server
	printf("Max Clients: ");
	std::cin >> this->mMaxClients;
	fgets(str, 512, stdin); // clean up input

}

Network::Network* Server::createNetwork() {
	// Create and return the appropriate network object
	return new Network::NetworkServer(this->mPort, this->mMaxClients);
}
