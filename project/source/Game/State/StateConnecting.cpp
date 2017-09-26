#include "Game\State\StateConnecting.h"

#include "Game\State\StateGame.h"
#include "Network\Net.h"

StateConnecting::StateConnecting() {

}

StateConnecting::~StateConnecting() {

}

void StateConnecting::queueNextGameState() {
	//mNext = ;
}

void StateConnecting::onEnterFrom(StateApplication *previous) {
	StateApplication::onEnterFrom(previous);

	// Initialize the network
	bool isHost = this->mData.network->networkType == StateNetwork::HOST; // will either by HOST or PEER

	//StateGame *nextState = StateGame::create(false);
	//new Net(nextState, true);

}

void StateConnecting::updateNetwork() {

}

void StateConnecting::handlePacket(PacketInfo *info) {

}

void StateConnecting::updateGame() {

}

void StateConnecting::render() {

}
