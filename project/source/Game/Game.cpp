/*
	Dustin Yost 0984932
	EGP-405-02
	Project 1
	09/21/2017
	We certify that this work is entirely our own.
	The assessor of this project may reproduce this project and provide
	copies to other academic staff, and/or communicate a copy of this project
	to a plagiarism-checking service, which may retain a copy of
	the project on its database.
*/
#include "Game\Game.h"

#include "Network\Framework.h"
#include "Network\Server.h"
#include "Network\Client.h"
#include "Game\Network\ServerPackets.h"
#include "Game\Network\ClientPackets.h"

#include "Game\State\StateApplication.h"
#include "Game\State\StateLobby.h"

// Author: Dustin Yost
	
Game::Game() {
	mpNetworkFramework = NULL;
}

Game::~Game() {
	if (mpNetworkFramework != NULL) {
		delete mpNetworkFramework;
		mpNetworkFramework = NULL;
	}
	delete mpStateGame;
}

void Game::run() {
	this->startup();
	this->runLoop();
}

void Game::startup() {
	
	this->mpStateGame = new StateLobby();

	HWND consoleWindow = GetConsoleWindow();
	this->mpStateGame->setWindow(consoleWindow);

	SetWindowLongPtr(consoleWindow, GWL_STYLE, GetWindowLongPtr(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	// yOffset, xOffset, height, width
	RECT rect = { 200, 200, 600, 800 };
	this->mpStateGame->console()->setSize(rect);
	MoveWindow(consoleWindow, rect.top, rect.left, rect.bottom, rect.right, TRUE);

	this->mpStateGame->console()->showConsoleCursor(false);

}

void Game::setNetworkType(bool isClient, FrameworkData data) {
	assert(mpNetworkFramework == NULL);

	// Create the framework peer
	if (isClient) {
		mpNetworkFramework = new Client(new ClientPackets(), data);
	}
	else {
		mpNetworkFramework = new Server(new ServerPackets(), data);
	}

	mpNetworkFramework->startup();
}

StateApplication* Game::getGameState() {
	return this->mpStateGame;
}

void Game::runLoop() {
	// Search for packet messages
	while (this->mpStateGame->mRunning)
	{
		this->mpStateGame->updateInput();
		this->mpStateGame->updateNetwork();
		this->mpStateGame->updateGame();
		this->mpStateGame->render();

		// Handle state transitions
		if (this->mpStateGame->hasNext()) {
			// Cache the previous state
			StateApplication *tmp = this->mpStateGame;
			// Notify previous state of exit
			this->mpStateGame->onExit();
			// Get the next state
			this->mpStateGame = this->mpStateGame->getNext();
			// Notify next state of entrance from previous state
			this->mpStateGame->onEnterFrom(tmp);
		}
	}
}

void Game::updateState() {

	// Handle queue to the state machine
	//this->processStateQueue();

	// Handle updating state classes
	//this->mpStateMachine->update(this);

	// Update game state data to follow the state machine
	/*
	if (this->mpStateGame->stateNext != StateGame::NONE) {
		this->mpStateGame->stateCurrent = this->mpStateGame->stateNext;
		this->mpStateGame->stateNext = StateGame::NONE;
	}
	//*/

}

void Game::updateNetwork() {
	
	/*
	if (!this->mpStateGame->running && this->mpNetworkFramework != NULL) {
		this->mpNetworkFramework->onExit();
	}

	if (this->mpNetworkFramework != NULL) {
		this->mpNetworkFramework->update(this->mpStateGame->network);
	}
	//*/

}
