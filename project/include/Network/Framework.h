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
#pragma once

#include "network\Network.h"

// Author: Dustin Yost
class Framework {

protected:
	bool mIsServer;
	unsigned int mPort;

	Network::Network *mpNetwork;
	Network::PacketHandler *mpPacketHandler;

	// Prompts the user for required information for initializing a client or server
	virtual void prompt();

	// Creates a network object based on user preference
	virtual Network::Network* createNetwork() { return NULL; };

public:
	Framework(bool isServer, Network::PacketHandler* packetHandler);
	virtual ~Framework();

	void startup();

	// Runs the game loop
	void update();
	void onExit();

};
