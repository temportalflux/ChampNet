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
#include "Network\Client.h"

// Author: Dustin Yost
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
