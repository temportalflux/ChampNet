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

#include "Game\InputSystem.h"
#include "Game\State\StateMachine.h"
#include "Game\State\StateLogin.h"

#include "Game\State\State.h"
#include "Game\State\StateApplication.h"
#include "Game\State\StateLobby.h"

// Author: Dustin Yost
	
Game::Game() {
	mpNetworkFramework = NULL;
	mpSystemInput = new InputSystem();
	mpStateMachine = new StateMachine();
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
	/*
	if (mpStateMachine != NULL) {
		delete mpStateMachine;
		mpStateMachine = NULL;
	}
	//*/
}

void Game::run() {
	this->startup();
	this->runLoop();
}

void Game::startup() {
	//this->mpStateGame->running = true;
	//this->mpStateGame->stateNext = StateApplication::LOGIN;
	StateLobby stateLobby[1];
	this->mpStateGame = stateLobby;
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

		// Update the state
		if (this->mpStateGame->hasNext()) {
			StateApplication::Data stateData;
			this->mpStateGame->onExit(&stateData);
			// TODO: finish me
		}
	}
}

void Game::updateState() {

	// Handle queue to the state machine
	this->processStateQueue();

	// Handle updating state classes
	this->mpStateMachine->update(this);

	// Update game state data to follow the state machine
	/*
	if (this->mpStateGame->stateNext != StateGame::NONE) {
		this->mpStateGame->stateCurrent = this->mpStateGame->stateNext;
		this->mpStateGame->stateNext = StateGame::NONE;
	}
	//*/

}

void Game::queueState(StateApplication::EnumState stateNext) {
	this->mpStateGame->stateNext = stateNext;
}

void Game::processStateQueue() {
	switch (this->mpStateGame->stateNext) {
		case StateApplication::LOGIN:
			this->mpStateMachine->queueState(new StateLogin());
			break;
		default:
			break;
	}
}

void Game::updateNetwork() {
	
	if (!this->mpStateGame->running && this->mpNetworkFramework != NULL) {
		this->mpNetworkFramework->onExit();
	}

	if (this->mpNetworkFramework != NULL) {
		this->mpNetworkFramework->update(this->mpStateGame->network);
	}

}
