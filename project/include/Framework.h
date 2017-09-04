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
#pragma once

#include "network\Network.h"
#include "PacketHandlerImpl.h"
#include "packets\Packet.h"

class Framework {

private:
	bool isServer;
	unsigned int port;
	unsigned int maxClients;
	std::string address;

	Network::Network *network;
	NetworkImpl::PacketHandler *packetHandler;
	void *consoleWindow;

public:
	Framework();
	~Framework();

private:

	// Creates a network object based on user preference
	Network::Network* createNetwork();

	// Prompts the user for required information for initializing a client or server
	void prompt();
	
	// Handles checking of input
	void input(bool &shouldExit);

public:

	// Runs the game loop (consoleWindow points to the current console)
	void run();

};
