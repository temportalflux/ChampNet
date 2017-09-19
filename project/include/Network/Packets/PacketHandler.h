#pragma once

#include "Network\Packets\PacketInfo.h"

// Author: Dustin Yost
namespace Network {

	class Network;

	class PacketHandler {

	public:

		virtual void subscribeTo(Network *network) = 0;

		// Handle incoming packets
		virtual void handlePacketData(PacketInfo info) = 0;

		virtual void onExit() = 0;

	};

};
