#include "Game\State\StateGameNetwork.h"

#include "Network\PacketInfo.h"
#include "Network\Net.h"
#include "Game\Packets\Packet.h"
#include "Game\State\StateConnecting.h"

StateGameNetwork::StateGameNetwork(Net *network, NetAddress *peerAddress) : StateGame(), mpNetwork(network), mpAddressPeer(peerAddress) {
	// Set the current ID to player 2
	mPlayerID = network->isServer() ? PLAYER_1 : PLAYER_2;
}

StateGameNetwork::~StateGameNetwork() {
	// Clean-up network and address vars
	if (mpNetwork != NULL) {
		delete mpNetwork;
		mpNetwork = NULL;
	}
	if (mpAddressPeer != NULL) {
		delete mpAddressPeer;
		mpAddressPeer = NULL;
	}
}

void StateGameNetwork::onEnterFrom(StateApplication *previous) {
	StateGame::onEnterFrom(previous);

	this->mIsPlayingAgain = EnumPlayAgain::UNDECIDED;
	this->mOpponentLeft = false;

	SetConsoleTitle(this->mpNetwork->isServer() ? "Host" : "Peer");

}

/* Author: Dustin Yost
 * Handles switch from this state, to either Connecting or Lobby states
 */
void StateGameNetwork::queueNextGameState() {
	// If we have stayed but the opponent has left
	if (mIsPlayingAgain == PLAY) { // setup a HOST using the already defined parameters
		this->mData.network->networkType = StateNetwork::NetworkType::HOST;
		mNext = new StateConnecting();
	}
	else {
		// We have left - do default behavior
		StateGame::queueNextGameState();
	}
}

/* Author: Dustin Yost
 * Handles prep for when the state is exitting
 */
void StateGameNetwork::onExit() {
	this->sendToPeer(ID_PLAYER_LEFT);
	this->mpNetwork->disconnect();
}

/* Author: Dustin Yost
 * Update the network interface
 */
void StateGameNetwork::updateNetwork() {
	this->mpNetwork->update();
}

/* Author: Dustin Yost
 * Handles all incoming packets - as BOTH a Host (Server) or Peer (Client)
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
			if (mIsPlayingAgain == PLAY) {
				// Tell opponent to start
				this->sendToPeer(ID_START_GAME);
				// Switch player ID
				//mPlayerID = this->getInvertID();
				// Start a new game
				this->startNewGame();
			}
			break;
		case ID_START_GAME:
			// Switch player ID
			//mPlayerID = this->getInvertID();
			// Start a new game
			this->startNewGame();
			break;
		case ID_PLAYER_LEFT:
			mOpponentLeft = true;
			if (mIsPlayingAgain != UNDECIDED) {
				this->queueNextGameState(); // Queue waiting state
			}
			break;
		default:
			break;
	}
}

/* Author: Dustin Yost
 * Sends some packet ID to the connected peer
 */
void StateGameNetwork::sendToPeer(unsigned char id) {
	this->mpNetwork->sendTo(PacketNotification{ id }, this->mpAddressPeer->address);
}

/* Author: Dustin Yost
 * Return the opposite ID of mPlayerID
 */
StateGame::PlayerIdentifier StateGameNetwork::getInvertID() {
	return mPlayerID == PLAYER_1 ? PLAYER_2 : PLAYER_1;
}

/* Author: Dustin Yost
 * Sets the player's icon in a specfied slot,
 * and sets the next player.
 * Returns the player that won, NONE if no one has won yet.
 * Sends a packet to the peer that we have committed a move.
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
	mIsWaitingForMove = true;
}

void StateGameNetwork::selectPlayAgain(bool playAgain) {
	this->mIsPlayingAgain = playAgain ? EnumPlayAgain::PLAY : EnumPlayAgain::QUIT;
	if (!mOpponentLeft && playAgain) {
		this->sendToPeer(ID_PLAY_AGAIN);
	}
	else {
		this->queueNextGameState();
	}
}

bool StateGameNetwork::validate(int slot, PlayerIdentifier player)
{
	return mPlayerID == mCurrentPlayer && StateGame::validate(slot, player);
}
