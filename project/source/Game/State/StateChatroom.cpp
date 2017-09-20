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

void StateChatroom::handlePacket(Network::PacketInfo info) {
	this->mPacketInputQueue.push_back(info);
}

void StateChatroom::updateGame() {

	Network::PacketInfo info;
	while (this->mPacketInputQueue.size() > 0) {
		this->doHandlePacket(this->mPacketInputQueue.front());
		this->mPacketInputQueue.pop_front();
	}

}
