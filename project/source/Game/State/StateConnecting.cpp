#include "Game\State\StateConnecting.h"

#include "Game\State\StateGameNetwork.h"
#include "Network\Net.h"
#include "Game\Packets\Packet.h"

StateConnecting::StateConnecting() {

}

StateConnecting::~StateConnecting() {

}

void StateConnecting::onEnterFrom(StateApplication *previous) {
	StateApplication::onEnterFrom(previous);

	// Initialize the network
	// will either by HOST or PEER
	bool isHost = this->mData.network->networkType == StateNetwork::HOST; 

	// We are not waiting for more input, so init the networks
	this->mpNetwork = new Net(this, isHost);
	if (isHost) { 
		// Create a server, referencing this state as the handler
		this->mpNetwork->initServer(this->mData.network->networkInfo.port, 1); // only 1 may connect
	}
	else {
		// Create a client (peer), referencing this state as the handler
		this->mpNetwork->initClient();
		this->mpNetwork->connectToServer(std::string(this->mData.network->networkInfo.serverAddress), this->mData.network->networkInfo.port);
	}

}

void StateConnecting::updateNetwork() {
	this->mpNetwork->update();
}

/* Author: Dustin Yost
	Handles all incoming packets - as BOTH a Host (Server) or Peer (Client)
*/
void StateConnecting::handlePacket(PacketInfo *info) {
	if (this->mpNetwork->isServer()) {
		this->handlePacketServer(info);
	}
	else {
		this->handlePacketClient(info);
	}
}

/* Author: Dustin Yost
	Handles all incoming packets - as a Host (Server)
*/
void StateConnecting::handlePacketServer(PacketInfo *info) {
	switch (info->getPacketType()) {
		case ID_USER_CONNECTED:
			// Peer has joined...

			// Notify them that we, the host, are ready to play
			this->mpNetwork->sendTo(PacketNotification{ ID_START_GAME }, info->address);

			// Start the game
			this->queueNextGameState();
			break;
		default:
			break;
	}
}

/* Author: Dustin Yost
	Handles all incoming packets - as a Peer (Client)
*/
void StateConnecting::handlePacketClient(PacketInfo *info) {
	switch (info->getPacketType()) {
		case ID_CONNECTION_REQUEST_ACCEPTED:
			// We HAVE connected
			// Notify HOST that we are ready
			this->mpNetwork->sendTo(PacketNotification{ ID_USER_CONNECTED }, info->address);
			break;
		case ID_START_GAME:
			// Host has acknowledged and the game is starting
			this->queueNextGameState();
			break;
		default:
			break;
	}
}

void StateConnecting::queueNextGameState() {
	// Create the next state
	StateGameNetwork *next = new StateGameNetwork(this->mpNetwork);

	// Set the handler of the network to be the next state (not the current state)
	this->mpNetwork->setHandler(next);

	// Remove reference to the network
	this->mpNetwork = NULL;

	// Set the next state
	mNext = next;
}

void StateConnecting::updateGame() {
	// STUB: Unneeded
}

void StateConnecting::render() {
	// STUB: Unneeded
	//this->renderConsole();
}
