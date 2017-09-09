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
		typedef char* UserName;
		typedef RakNet::SystemAddress UserAddress;
		typedef std::map<UserName, UserAddress> UserNameToAddress;
		typedef std::map<UserAddress, UserName> UserAddressToName;

	private:
		// Author: Dustin Yost
		UserNameToAddress mMapNameToAddress;
		// Author: Dustin Yost
		UserAddressToName mMapAddressToName;

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
