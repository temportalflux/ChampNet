#pragma once

#include "Game\State\StateApplication.h"

// Author: Dustin Yost
class StateLobby : public StateApplication {

private:

	enum EnumLoginPhase {
		NETWORK_TYPE, // Server or Client
		NETWORK_PORT, // Port #
		ADDRESS, // Client: The address string
		MAX_CLIENTS // Server: Max Clients
	} mPhase;

	virtual void onEnterFrom(StateApplication *previous);
	void updateNetwork();
	void updateGame();
	void render();

	bool updateForInput(std::string &latestLine, bool allowEmptyLines);
	void queueStateChatroom();

};
