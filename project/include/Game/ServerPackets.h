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

#include <map>

namespace Game {

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
		// Author: Dustin Yost
		UserIDToAddress mMapIDToAddress;
		// Author: Dustin Yost
		UserAddressToID mMapAddressToID;
		// Author: Dustin Yost
		UserIDToName mMapIDToName;

	public:
		ServerPackets();
		virtual ~ServerPackets();

		// Author: Dustin Yost
		virtual void subscribeTo(Network::Network *network);

		// Author: Dustin Yost
		// Handle incoming packets
		virtual void handlePacketData(Network::PacketInfo info);

		// Author: Dustin Yost
		virtual void onExit() {}

	};

};
