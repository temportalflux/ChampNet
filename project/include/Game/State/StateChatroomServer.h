#pragma once

#include "Game\State\StateChatroom.h"

class StateChatroomServer : public StateChatroom {

private:
	//void updateGame();
	void doHandlePacket(Network::PacketInfo info) override;
	void render();

	void sendPacket(Packet packet) override;

};
