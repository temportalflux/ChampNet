/*
Dustin Yost 0984932
Jon Trusheim 0979380
EGP-405-02
Project 1
09/21/2017
We certify that this work is entirely our own.
The assessor of this project may reproduce this project and provide
copies to other academic staff, and/or communicate a copy of this project
to a plagiarism-checking service, which may retain a copy of
the project on its database.
*/

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
