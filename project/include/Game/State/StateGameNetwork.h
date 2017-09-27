#pragma once

#include "Game\State\StateGame.h"
#include "Network\MessageHandler.h"

class Net;
struct NetAddress;

class StateGameNetwork : public StateGame, public MessageHandler {

private:
	Net *mpNetwork;
	NetAddress *mpAddressPeer;
	PlayerIdentifier mPlayerID;

	void updateNetwork();

	/* Author: Dustin Yost
		Handles all incoming packets - as BOTH a Host (Server) or Peer (Client)
	*/
	void handlePacket(PacketInfo *info);

	void sendToPeer(unsigned char id);

public:

	StateGameNetwork(Net *network, NetAddress *peerAddress);
	virtual ~StateGameNetwork();

	/* Author: Dustin Yost
		Sets the player's icon in a specfied slot,
		and sets the next player.
		Returns the player that won, NONE if no one has won yet.
		Sends a packet to the peer that we have committed a move.
	*/
	virtual PlayerIdentifier commitMove(int slot, PlayerIdentifier player);

	/*
	* Handles setting a wait flag when the user has committed their turn 
	*/
	void onMoveCommitted();

};
