#pragma once

#include "Game\State\StateGame.h"
#include "Network\MessageHandler.h"

class StateGameNetwork : public StateGame, public MessageHandler {

private:

	void updateNetwork();
	void handlePacket(PacketInfo *info);

};
