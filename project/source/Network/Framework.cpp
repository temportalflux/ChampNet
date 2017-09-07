/*
	Dustin Yost 0984932
	EGP-405-02
	Lab1
	09/03/2017
	We certify that this work is entirely our own.
	The assessor of this project may reproduce this project and provide
	copies to other academic staff, and/or communicate a copy of this project
	to a plagiarism-checking service, which may retain a copy of
	the project on its database.
*/
#include "Network\Framework.h"

#include <iostream>

Framework::Framework(bool isServer, Network::PacketHandler* packetHandler) : mIsServer(isServer) {
	this->mpPacketHandler = packetHandler;
}

Framework::~Framework() {
	// Destroy the connection
	delete this->mpNetwork;
	delete this->mpPacketHandler;
}

void Framework::startup() {

	// Prompt user for required information
	this->prompt();

	// Create the network and a packethandler for the network
	this->mpNetwork = this->createNetwork();
	this->mpPacketHandler->subscribeTo(this->mpNetwork);

	// Initialize the socket connection
	this->mpNetwork->startup();

	// Notify user that the game has started
	printf("Starting the %s.\n", this->mpNetwork->isServer ? "server" : "client");

	// Setup the server or Connect from client to server
	this->mpNetwork->connect();

}

void Framework::prompt() {
	char str[512];

	printf("Server Port: ");
	std::cin >> this->mPort;
	fgets(str, 512, stdin); // clean up input

}

void Framework::update() {
	// Update the network
	this->mpNetwork->update();
}

void Framework::onExit() {
	this->mpPacketHandler->onExit();
}
