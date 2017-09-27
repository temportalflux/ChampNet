#pragma once

#include "Game\State\StateApplication.h"
#include "Network\MessageHandler.h"

class Net;
struct NetAddress;

class StateConnecting : public StateApplication, public MessageHandler {

private:
	// The network to be handed over on exit
	Net *mpNetwork;
	// The address of the other player
	NetAddress *mpAddressPeer;

	virtual void queueNextGameState() override;

	virtual void onEnterFrom(StateApplication *previous);

	void updateNetwork();

	/* Author: Dustin Yost
		Handles all incoming packets - as BOTH a Host (Server) or Peer (Client)
	*/
	void handlePacket(PacketInfo *info);
	/* Author: Dustin Yost
		Handles all incoming packets - as a Host (Server)
	*/
	void handlePacketServer(PacketInfo *info);
	/* Author: Dustin Yost
		Handles all incoming packets - as a Peer (Client)
	*/
	void handlePacketClient(PacketInfo *info);
	void updateGame();
	void render();

public:

	StateConnecting();
	virtual ~StateConnecting();

};
