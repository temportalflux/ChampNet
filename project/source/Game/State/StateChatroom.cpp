#include "Game\State\StateChatroom.h"

#include "Network\Framework.h"
#include "Network\Server.h"
#include "Network\Client.h"
#include "Game\Network\ServerPackets.h"
#include "Game\Network\ClientPackets.h"

StateChatroom::StateChatroom() : StateApplication() {
	mpNetworkFramework = NULL;
	mPacketInputQueue = std::deque<Network::PacketInfo>();
}

StateChatroom::~StateChatroom() {
	if (mpNetworkFramework != NULL) {
		mpNetworkFramework->onExit();
		delete mpNetworkFramework;
		mpNetworkFramework = NULL;
	}
}

void StateChatroom::onEnterFrom(StateApplication *previous) {
	StateApplication::onEnterFrom(previous);

	system("cls");

	// Startup the network
	// Create the framework peer
	if (this->mData.network->isServer) {
		mpNetworkFramework = new Server(new ServerPackets(this), this->mData.network->networkInfo);
	}
	else {
		mpNetworkFramework = new Client(new ClientPackets(this), this->mData.network->networkInfo);
	}

	mpNetworkFramework->startup();

}

void StateChatroom::updateNetwork() {
	this->mpNetworkFramework->update();
}

/* Author: Dustin Yost
Handle packets which have been recieved since last update
*/
void StateChatroom::handlePacket(Network::PacketInfo info) {
	// Called from the updateNetwork loop
	// push a packet into the queue for later
	this->mPacketInputQueue.push_back(info);
}

void StateChatroom::updateGame() {

	// Author: Dustin Yost
	// Handle updates from the network packets
	Network::PacketInfo info;
	while (this->mPacketInputQueue.size() > 0) {
		// Pass off the packet for handling in children classes
		this->doHandlePacket(this->mPacketInputQueue.front());
		// Pop the first packet (we are done with it)
		this->mPacketInputQueue.pop_front();
	}

}

/* Author: Dustin Yost
Pushes a text message to the record of messages to display
*/
void StateChatroom::pushMessage(const std::string &msg) {
	// Push the message into the buffer
	this->mData.display->textRecord.push_back(std::string(msg.c_str()));
}
