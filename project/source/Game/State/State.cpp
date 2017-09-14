#include "Game\State\State.h"
#include "Game\Game.h"

State::~State() {
}

void State::onEnter(Game* game) {
}

void State::update(Game* game) {
	StateGame* state = game->getGameState();
	this->handleInput(state->input);
	this->handleNetwork(state->network);
	this->handleGameUpdates(state, game);
}

void State::handleInput(StateInput* state) {}

void State::handleNetwork(StateNetwork* state) {}

void State::handleGameUpdates(StateGame* state, Game* game) {}

void State::onExit(Game* game) {
}
