#include "Game\State\StateChatroom.h"

#include "Network\Framework.h"
#include "Network\Server.h"
#include "Network\Client.h"
#include "Game\Network\ServerPackets.h"
#include "Game\Network\ClientPackets.h"

StateChatroom::StateChatroom() : StateApplication() {
	mpNetworkFramework = NULL;
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
		mpNetworkFramework = new Server(new ServerPackets(), this->mData.network->networkInfo);
	}
	else {
		mpNetworkFramework = new Client(new ClientPackets(), this->mData.network->networkInfo);
	}

	mpNetworkFramework->startup();

}

void StateChatroom::updateNetwork() {

	// TODO: this->mpNetworkFramework->update();

}

void StateChatroom::updateGame() {

}

void StateChatroom::render() {

}
