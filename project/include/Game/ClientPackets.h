#pragma once

#include "Network\PacketManager.h"

namespace Game {

	class ClientPackets : public Network::PacketHandler {

	private:
		Network::Network* mpNetwork;
		RakNet::SystemAddress mAddressServer;

	public:
		ClientPackets();
		virtual ~ClientPackets();

		virtual void subscribeTo(Network::Network *network);

		// Handle incoming packets
		virtual void handlePacketData(Network::PacketInfo info);
		
		virtual void onExit();

	};


};