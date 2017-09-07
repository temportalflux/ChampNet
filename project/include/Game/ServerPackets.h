#pragma once

#include "Network\PacketManager.h"

namespace Game {

	class ServerPackets : public Network::PacketHandler {

	private:

	public:
		ServerPackets();
		virtual ~ServerPackets();

		virtual void subscribeTo(Network::Network *network);

		// Handle incoming packets
		virtual void handlePacketData(Network::PacketInfo info);

		virtual void onExit() {}

	};

};
