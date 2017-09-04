/*
	Dustin Yost 0984932
	EGP-405-02
	Lab1
	09/03/2017
	We certify that this work is entirely our own.
	The assessor of this project may reproduce this project and provide
	copies to other academic staff, and/or communicate a copy of this project
	to a plagiarism-checking service, which may retain a copy of
	the project on its database.
*/
#pragma once

#include "network\PacketManager.h"

namespace NetworkImpl {

	class PacketHandler : public Network::PacketHandler {

	private:
		bool isServer;

	public:
		PacketHandler(bool isServer);

		void subscribeTo(Network::Network *network);

		void handlePacketData(Network::PacketInfo info);
		void handlePacketServer(Network::PacketInfo info);
		void handlePacketClient(Network::PacketInfo info);

	};

}
