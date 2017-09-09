/*
	Dustin Yost 0984932
	EGP-405-02
	Project 1
	09/21/2017
	We certify that this work is entirely our own.
	The assessor of this project may reproduce this project and provide
	copies to other academic staff, and/or communicate a copy of this project
	to a plagiarism-checking service, which may retain a copy of
	the project on its database.
*/
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
