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

	/* Author: Dustin Yost
	 * Handles switch from this state
	 */
	virtual void queueNextGameState() override;

	virtual void onEnterFrom(StateApplication *previous);

	/* Author: Dustin Yost
	 * Update the network interface
	 */
	void updateNetwork();

	/* Author: Dustin Yost
	 * Handles all incoming packets - as BOTH a Host (Server) or Peer (Client)
	 */
	void handlePacket(PacketInfo *info) override;
	/* Author: Dustin Yost
	 * Handles all incoming packets - as a Host (Server)
	 */
	void handlePacketServer(PacketInfo *info);
	/* Author: Dustin Yost
	 * Handles all incoming packets - as a Peer (Client)
	 */
	void handlePacketClient(PacketInfo *info);
	void updateGame();
	void render();

public:

	StateConnecting();
	virtual ~StateConnecting();

};
