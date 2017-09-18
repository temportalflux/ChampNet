#pragma once

#include "Game\State\StateApplication.h"

class StateLobby : public StateApplication {

private:

	enum EnumLogin {
		NETWORK_TYPE, // Server or Client
		NETWORK_PORT, // Port #
		ADDRESS, // Client: The address string
		MAX_CLIENTS // Server: Max Clients
	} mPhase;

	void updateNetwork();
	void updateGame();
	void render();

};
