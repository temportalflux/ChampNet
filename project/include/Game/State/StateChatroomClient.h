#pragma once

#include "Game\State\StateChatroom.h"

class StateChatroomClient : public StateChatroom {

private:
	//void updateGame();
	void doHandlePacket(Network::PacketInfo info) override;
	void render();

};
