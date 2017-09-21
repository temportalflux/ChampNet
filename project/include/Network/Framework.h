/*
Dustin Yost 0984932
Jon Trusheim 0979380
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

#include "Network\FrameworkDefines.h"
#include "Network\NetworkDefines.h"
#include <RakNet\PacketPriority.h>

namespace Network {
	class Network;
	class PacketHandler;
};
namespace RakNet {
	struct SystemAddress;
};

// Author: Dustin Yost
class Framework {

protected:
	bool mIsServer;
	unsigned int mPort;

	Network::Network *mpNetwork;
	Network::PacketHandler *mpPacketHandler;

	// Creates a network object based on user preference
	virtual Network::Network* createNetwork() = 0;

public:
	Framework(bool isServer, Network::PacketHandler* packetHandler);
	virtual ~Framework();

	void startup();

	// Runs the game loop
	void update();
	void onExit();

	Network::Network *getNetwork();
	void sendTo(char *data, unsigned int size, RakNet::SystemAddress *address,
		PacketPriority priority, PacketReliability reliability,
		char channel, bool broadcast
	);

};
