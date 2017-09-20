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

#include "Network\Packets\PacketHandler.h"

#include <map>

class MessageHandler;

// Author: Dustin Yost
class ServerPackets : public Network::PacketHandler {
	// Author: Dustin Yost
	typedef unsigned int UserID;
	typedef RakNet::SystemAddress UserAddress;
	typedef char* UserName;
	typedef std::map<UserID, UserAddress> UserIDToAddress;
	typedef std::map<UserAddress, UserID> UserAddressToID;
	typedef std::map<UserID, UserName> UserIDToName;

private:
	Network::Network* mpNetwork;
	// Author: Dustin Yost
	UserIDToAddress mMapIDToAddress;
	// Author: Dustin Yost
	UserAddressToID mMapAddressToID;
	// Author: Dustin Yost
	UserIDToName mMapIDToName;

	MessageHandler *mpMessageHandler;

public:
	ServerPackets(MessageHandler *messageHandler);
	virtual ~ServerPackets();

	virtual void subscribeTo(Network::Network *network);

	// Handle incoming packets
	virtual void handlePacketData(Network::PacketInfo info);

	virtual void onExit() {}

	// Find the next available ID in the mMapIDToName map (seraches for empty slots)
	// Returns the first available empty slot, or the number of entries in the map if there are no empty slots
	UserID getNextFreeID();
	// Returnst the user name for the user id
	UserName getNameFromID(UserID id);

};
