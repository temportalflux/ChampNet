#pragma once

#include "Network\PacketManager.h"

namespace Game {

	class ClientPackets : public Network::PacketHandler {
		typedef unsigned int UserID;

	private:
		Network::Network* mpNetwork;
		RakNet::SystemAddress mAddressServer;
		UserID mID;

	public:
		ClientPackets();
		virtual ~ClientPackets();

		virtual void subscribeTo(Network::Network *network);

		// Handle incoming packets
		virtual void handlePacketData(Network::PacketInfo info);
		
		virtual void onExit();

	};


};