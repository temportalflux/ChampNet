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

struct FrameworkData {
	unsigned int port;
	unsigned int maxClients;
	char serverAddress[15];// ***.***.***.*** 12+3=15
};

struct StateNetwork;

// Author: Dustin Yost
class Framework {

protected:
	bool mIsServer;
	unsigned int mPort;

	Network::Network *mpNetwork;
	Network::PacketHandler *mpPacketHandler;

	// Creates a network object based on user preference
	virtual Network::Network* createNetwork() { return NULL; };

public:
	Framework(bool isServer, Network::PacketHandler* packetHandler);
	virtual ~Framework();

	void startup();

	// Runs the game loop
	void update(StateNetwork* stateNetwork);
	void onExit();

};
