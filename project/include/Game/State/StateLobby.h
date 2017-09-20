#pragma once

#include "Game\State\StateApplication.h"

// Author: Dustin Yost
class StateLobby : public StateApplication {

private:

	enum EnumLoginPhase {
		NETWORK_TYPE, // Server or Client
		NETWORK_PORT, // Port #
		ADDRESS, // Client: The address string
		MAX_CLIENTS, // Server: Max Clients
		USERNAME, // users username
		WAIT_FOR_USERNAME_VALIDATION
	} mPhase;

	virtual void onEnterFrom(StateApplication *previous);
	void updateNetwork();
	void updateGame();
	void render();

	void queueStateChatroom();

};
