#include "Game\State\StateGame.h"

#include "Game\State\StateGameLocal.h"
#include "Game\State\StateGameNetwork.h"
#include "Game\State\StateLobby.h"

/* Author: Dustin Yost
	Creates a local/network game state
*/
StateGame* StateGame::create(bool isLocalGame) {
	if (isLocalGame) {
		return new StateGameLocal();
	}
	else {
		return new StateGameNetwork();
	}
}

StateGame::StateGame() {
	// mBoardState = array; created already b/c object
	for (int slot = 0; slot < BOARD_SLOTS; slot++) {
		mBoardState[slot] = PlayerIdentifier::NONE;
	}
	mCurrentPlayer = PlayerIdentifier::PLAYER_1;
}

StateGame::~StateGame() {

}

void StateGame::queueNextGameState() {
	this->mNext = new StateLobby();
}

void StateGame::onEnterFrom(StateApplication *previous) {

}

void StateGame::updateGame() {

}

void StateGame::render() {

}

/* Author: Dustin Yost
Validate the player's selection of a slot
*/
bool StateGame::validate(int slot, PlayerIdentifier player) {
	// False if:
	// - invalid slot
	// - slot not empty
	return (slot >= 0 && slot < BOARD_SLOTS) &&
		mBoardState[slot] == PlayerIdentifier::NONE;
}
