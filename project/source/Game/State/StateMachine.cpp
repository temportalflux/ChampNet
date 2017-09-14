#include "Game\State\StateMachine.h"
#include <cstdlib> // NULL

#include "Game\State\State.h"
#include "Game\State\StateLogin.h"
#include "Game\Game.h"

StateMachine::StateMachine() {
	mpState = NULL;
	mStateCurrent = EnumState::NONE;
	mStateNext = EnumState::NONE;
}

StateMachine::~StateMachine() {
	this->cleanState();
}

void StateMachine::queueState(EnumState state) {
	this->mStateNext = state;
}

void StateMachine::update(Game* game) {
	if (mStateNext != mStateCurrent) {

		// Process exit
		if (this->mpState != NULL) {
			this->mpState->onExit(game);
		}

		// Exit the current state
		this->cleanState();

		// Create the next state
		this->mpState = this->createStateFor(mStateNext);
		this->mStateCurrent = this->mStateNext;

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

State* StateMachine::createStateFor(EnumState state) {
	switch (state) {
		case EnumState::LOGIN:
			return new StateLogin();
		default:
			return NULL;
	}
}
