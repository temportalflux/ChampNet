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
	if (mpStateMachine != NULL) {
		delete mpStateMachine;
		mpStateMachine = NULL;
	}
}

void Game::run() {
	this->startup();
	this->runLoop();
}

void Game::startup() {
	this->mpStateMachine->queueState(EnumState::LOGIN);
	this->mpStateMachine->update(this);
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
		
	this->mpStateMachine->update(this);
	this->mpNetworkFramework->update();
	this->mpSystemInput->update(shouldExit);

	if (preShouldExit != shouldExit) {
		this->mpNetworkFramework->onExit();
	}
}
