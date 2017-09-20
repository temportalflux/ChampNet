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

// Author: Jon Trusheim
void StateChatroom::updateGame() 
{

	Network::PacketInfo info;
	while (this->mPacketInputQueue.size() > 0) {
		this->doHandlePacket(this->mPacketInputQueue.front());
		this->mPacketInputQueue.pop_front();
	}

	std::string latestLine;
	if (this->updateForInput(latestLine, true))
	{
		if (latestLine.at(0) == '/') //checks to make sure that the first character is a /
		{
			if (latestLine.find("/help") == 0)
			{
				this->mData.display->textRecord.push_back("type /pm to message a specfic user");
			}
			else if (latestLine.find("/pm") == 0)
			{
				/*
				get input of the username
				string userName = input;
				check user name with server to see if it is valid




				*/
			}
			else
			{
				// tells the user that it was not a valid command
				this->mData.display->textRecord.push_back("Not a valid command");
			}
		}
	}
}


