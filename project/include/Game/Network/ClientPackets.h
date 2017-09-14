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

#include "Network\PacketManager.h"

// Author: Dustin Yost
class ClientPackets : public Network::PacketHandler {
	typedef unsigned int UserID;

private:
	// Author: Dustin Yost
	Network::Network* mpNetwork;
	// Author: Dustin Yost
	RakNet::SystemAddress mAddressServer;
	// Author: Dustin Yost
	UserID mID;

public:
	ClientPackets();
	virtual ~ClientPackets();

	// Author: Dustin Yost
	virtual void subscribeTo(Network::Network *network);

	// Author: Dustin Yost
	// Handle incoming packets
	virtual void handlePacketData(Network::PacketInfo info);

	// Author: Dustin Yost
	virtual void onExit();

};
