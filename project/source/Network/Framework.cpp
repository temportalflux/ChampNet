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
#include "Network\Framework.h"

#include <iostream>

// Author: Dustin Yost
Framework::Framework(bool isServer, Network::PacketHandler* packetHandler) : mIsServer(isServer) {
	this->mpPacketHandler = packetHandler;
}

Framework::~Framework() {
	// Destroy the connection
	delete this->mpNetwork;
	delete this->mpPacketHandler;
}

void Framework::startup() {

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

void Framework::update() {
	// Update the network
	this->mpNetwork->update();
}

void Framework::onExit() {
	this->mpPacketHandler->onExit();
}
