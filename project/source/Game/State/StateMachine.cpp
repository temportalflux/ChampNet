#include "Game\State\StateMachine.h"
#include <cstdlib> // NULL

#include "Game\State\State.h"
#include "Game\State\StateLogin.h"
#include "Game\Game.h"

StateMachine::StateMachine() {
	this->mpState = NULL;
	this->mpStateNext = NULL;
}

StateMachine::~StateMachine() {
	this->cleanState();
}

void StateMachine::queueState(State* nextState) {
	this->mpStateNext = nextState;
}

void StateMachine::update(Game* game) {
	if (this->mpStateNext != NULL) {

		// Process exit
		if (this->mpState != NULL) {
			this->mpState->onExit(game);
		}

		// Exit the current state
		this->cleanState();

		// Create the next state
		this->mpState = this->mpStateNext;

		// Process enter
		if (this->mpState != NULL) {
			this->mpState->onEnter(game);
		}

	}
	if (this->mpState != NULL) {
		this->mpState->update(game);
	}
}

void StateMachine::cleanState() {
	if (this->mpState != NULL) {
		delete mpState;
		this->mpState = NULL;
	}
}
