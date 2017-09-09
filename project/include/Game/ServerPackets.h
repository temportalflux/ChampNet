#pragma once

#include "Network\PacketManager.h"

#include <map>

namespace Game {

	class ServerPackets : public Network::PacketHandler {
		typedef char* UserName;
		typedef RakNet::SystemAddress UserAddress;
		typedef std::map<UserName, UserAddress> UserNameToAddress;
		typedef std::map<UserAddress, UserName> UserAddressToName;

	private:
		UserNameToAddress mMapNameToAddress;
		UserAddressToName mMapAddressToName;

	public:
		ServerPackets();
		virtual ~ServerPackets();

		virtual void subscribeTo(Network::Network *network);

		// Handle incoming packets
		virtual void handlePacketData(Network::PacketInfo info);

		virtual void onExit() {}

	};

};
