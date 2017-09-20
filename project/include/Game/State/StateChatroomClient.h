#pragma once

#include "Game\State\StateChatroom.h"

class StateChatroomClient : public StateChatroom {

private:
	RakNet::SystemAddress *mAddressServer;

	//void updateGame();
	void doHandlePacket(Network::PacketInfo *info) override;
	void render();
	
	void sendPacket(Packet packet) override;

};
