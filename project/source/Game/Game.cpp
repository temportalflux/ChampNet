#include "Game\Game.h"

#include "Network\Framework.h"
#include "Network\Server.h"
#include "Network\Client.h"
#include "Game\ServerPackets.h"
#include "Game\ClientPackets.h"

#include "Game\InputSystem.h"

namespace Game {

	Game::Game() {
		mpNetworkFramework = NULL;
		mpSystemInput = new InputSystem();
	}

	Game::~Game() {
		if (mpNetworkFramework != NULL) {
			delete mpNetworkFramework;
			mpNetworkFramework = NULL;
		}
		if (mpSystemInput != NULL) {
			delete mpSystemInput;
			mpSystemInput = NULL;
		}
	}

	void Game::run() {
		this->startup();
		this->runLoop();
	}

	void Game::startup() {
		char str[512];

		// Get whether to start as a client or server
		printf("(C) or (S)erver? ");
		fgets(str, 512, stdin);
		// true if the user input spcifies client
		bool isClient = (str[0] == 'c') || (str[0] == 'C');

		// Create the framework peer
		if (isClient) {
			mpNetworkFramework = new Client(new ClientPackets());
		}
		else {
			mpNetworkFramework = new Server(new ServerPackets());
		}

		mpNetworkFramework->startup();

	}

	void Game::runLoop() {
		// Search for packet messages
		bool shouldExit = false;
		while (!shouldExit)
		{
			this->update(shouldExit);
		}
	}

	void Game::update(bool &shouldExit) {
		bool preShouldExit = shouldExit;
		
		this->mpNetworkFramework->update();
		this->mpSystemInput->update(shouldExit);

		if (preShouldExit != shouldExit) {
			this->mpNetworkFramework->onExit();
		}
	}

}
