#pragma once

#include "Game\State\StateApplication.h"
#include "Network\MessageHandler.h"

class StateConnecting : public StateApplication, public MessageHandler {

private:

	virtual void queueNextGameState() override;

	virtual void onEnterFrom(StateApplication *previous);

	void updateNetwork();
	void handlePacket(PacketInfo *info);
	void updateGame();
	void render();

public:

	StateConnecting();
	virtual ~StateConnecting();

};
