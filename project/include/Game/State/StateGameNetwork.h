#pragma once

#include "Game\State\StateGame.h"
#include "Network\MessageHandler.h"

class Net;
struct NetAddress;

class StateGameNetwork : public StateGame, public MessageHandler {

private:
	// The network interface to send packets through
	Net *mpNetwork;
	// The connect peer address
	NetAddress *mpAddressPeer;
	// The ID of the current player
	PlayerIdentifier mPlayerID;
	// If we are waiting for a move from the opponent
	bool mIsWaitingForMove;
	// If we are ready to play again
	EnumPlayAgain mIsPlayingAgain;
	bool mOpponentLeft = false;

	virtual void onEnterFrom(StateApplication *previous);

	/* Author: Dustin Yost
	 * Handles switch from this state, to either Connecting or Lobby states
	 */
	void queueNextGameState() override;

	/* Author: Dustin Yost
	 * Handles prep for when the state is exitting
	 */
	void onExit() override;

	/* Author: Dustin Yost
	 * Update the network interface
	 */
	void updateNetwork();

	/* Author: Dustin Yost
	 * Handles all incoming packets - as BOTH a Host (Server) or Peer (Client)
	 */
	void handlePacket(PacketInfo *info);

	/* Author: Dustin Yost
	 * Sends some packet ID to the connected peer
	 */
	void sendToPeer(unsigned char id);

	/* Author: Dustin Yost
	 * Return the opposite ID of mPlayerID
	 */
	PlayerIdentifier getInvertID();

	/* Author: Dustin Yost
	 * Sets the network to play again
	 */
	void selectPlayAgain(bool playAgain) override;

	bool validate(int slot, PlayerIdentifier player) override;

public:

	StateGameNetwork(Net *network, NetAddress *peerAddress);
	virtual ~StateGameNetwork();

	/* Author: Dustin Yost
	 * Sets the player's icon in a specfied slot,
	 * and sets the next player.
	 * Returns the player that won, NONE if no one has won yet.
	 * Sends a packet to the peer that we have committed a move.
	 */
	virtual PlayerIdentifier commitMove(int slot, PlayerIdentifier player);

	/*
	 * Handles setting a wait flag when the user has committed their turn 
	 */
	void onMoveCommitted();

};
