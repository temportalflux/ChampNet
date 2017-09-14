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
Client::Client(Network::PacketHandler* packetHandler, FrameworkData data) : Framework(false, packetHandler) {
	this->mPort = data.port;
	this->mAddressServerStr = std::string(data.serverAddress);
}

Client::~Client() {
}

Network::Network* Client::createNetwork() {
	// Create and return the appropriate network object
	return new Network::NetworkClient(this->mAddressServerStr, this->mPort);
}
