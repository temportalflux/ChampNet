#pragma once

#include "Game\State\StateGame.h"
#include "Network\MessageHandler.h"

class Net;

class StateGameNetwork : public StateGame, public MessageHandler {

private:
	Net *mpNetwork;

	void updateNetwork();
	void handlePacket(PacketInfo *info);

public:

	StateGameNetwork(Net *network);
	virtual ~StateGameNetwork();

};
