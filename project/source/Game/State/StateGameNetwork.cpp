#include "Game\State\StateGameNetwork.h"

#include "Network\PacketInfo.h"
#include "Network\Net.h"
#include "Game\Packets\Packet.h"

StateGameNetwork::StateGameNetwork(Net *network, NetAddress *peerAddress) : StateGame(), mpNetwork(network), mpAddressPeer(peerAddress) {
	mPlayerID = PlayerIdentifier::PLAYER_2;
	mIsPlayingAgain = false;
}

StateGameNetwork::~StateGameNetwork() {
	if (mpNetwork != NULL) {
		delete mpNetwork;
		mpNetwork = NULL;
	}
	if (mpAddressPeer != NULL) {
		delete mpAddressPeer;
		mpAddressPeer = NULL;
	}
}

void StateGameNetwork::updateNetwork() {
	this->mpNetwork->update();
}

/* Author: Dustin Yost
	Handles all incoming packets - as BOTH a Host (Server) or Peer (Client)
*/
void StateGameNetwork::handlePacket(PacketInfo *info) {
	switch (info->getPacketType()) {
		// Fall down cases
		case ID_MOVE_SUBMIT_0: case ID_MOVE_SUBMIT_1: case ID_MOVE_SUBMIT_2:
		case ID_MOVE_SUBMIT_3: case ID_MOVE_SUBMIT_4: case ID_MOVE_SUBMIT_5:
		case ID_MOVE_SUBMIT_6: case ID_MOVE_SUBMIT_7: case ID_MOVE_SUBMIT_8:
			{
				// Get the slot by shifting down from the current packet ID
				int slot = info->getPacketType() - ID_MOVE_SUBMIT_0;
				// Case map the player ID
				PlayerIdentifier opponent = this->getInvertID();
				// Call super to avoid network call
				mWinner = StateGame::commitMove(slot, opponent);
			}
			break;
		case ID_PLAY_AGAIN:
			// If local use is ready to play again (otherwise we have not selected yet)
			if (mIsPlayingAgain) {
				// Tell opponent to start
				this->sendToPeer(ID_START_GAME);
				// Switch player ID
				mPlayerID = this->getInvertID();
				this->startNewGame();
			}
			break;
		case ID_START_GAME:
			this->startNewGame();
			break;
		case ID_PLAYER_LEFT:

			break;
		default:
			break;
	}
}

void StateGameNetwork::sendToPeer(unsigned char id) {
	this->mpNetwork->sendTo(PacketNotification{ id }, this->mpAddressPeer->address);
}

StateGame::PlayerIdentifier StateGameNetwork::getInvertID() {
	return mPlayerID == PLAYER_1 ? PLAYER_2 : PLAYER_1;
}

/* Author: Dustin Yost
	Sets the player's icon in a specfied slot,
	and sets the next player.
	Returns the player that won, NONE if no one has won yet.
	Sends a packet to the peer that we have committed a move.
*/
StateGame::PlayerIdentifier StateGameNetwork::commitMove(int slot, PlayerIdentifier player) {
	PlayerIdentifier winner = StateGame::commitMove(slot, player);
	this->sendToPeer(ID_MOVE_SUBMIT_0 + slot); // NOTE: Assumes the submition packet ids are in order from 0->8
	this->onMoveCommitted();
	return winner;
}

/*
* Handles setting a wait flag when the user has committed their turn
*/
void StateGameNetwork::onMoveCommitted() {

}
