#include "Game\State\StateGame.h"

#include "Game\State\StateGameLocal.h"
#include "Game\State\StateGameNetwork.h"
#include "Game\State\StateLobby.h"

StateApplication* StateGame::create(bool isLocalGame) {
	if (isLocalGame) {
		return new StateGameLocal();
	}
	else {
		return new StateGameNetwork();
	}
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
