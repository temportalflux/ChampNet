#include "Game\State\StateChatroom.h"

#include "Network\Framework.h"
#include "Network\Server.h"
#include "Network\Client.h"
#include "Game\Network\ServerPackets.h"
#include "Game\Network\ClientPackets.h"
#include "Game\Packets\Packet.h"
#include "Network\Packets\PacketInfo.h"

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

	this->pushMessage(std::string("Starting the ") + (this->mData.network->isServer ? "server" : "client"));

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

void StateChatroom::updateGame() 
{

	// Author: Dustin Yost
	// Handle updates from the network packets
	while (this->mPacketInputQueue.size() > 0) {
		// Pass off the packet for handling in children classes
		this->doHandlePacket(this->mPacketInputQueue.front());
		// Pop the first packet (we are done with it)
		this->mPacketInputQueue.pop_front();
	}

	// Author: Jon Trusheim
	std::string latestLine;
	if (this->updateForInput(latestLine, false))
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

Framework* StateChatroom::getFramework() {
	return mpNetworkFramework;
}

/* Author: Dustin Yost
Pushes a text message to the record of messages to display
*/
void StateChatroom::pushMessage(const std::string &msg) {
	// Push the message into the buffer
	this->mData.display->textRecord.push_back(std::string(msg.c_str()));
}

/* Author: Dustin Yost
Sends a packet to the server indicating a public message
*/
void StateChatroom::sendMessage(const std::string &message) {
	PacketString packet;
	packet.packetID = ID_CHAT_MESSAGE;
	strncpy(packet.content, message.c_str(), min(message.length(), PacketString::MAX_SIZE_CONTENT));
	this->sendPacket(packet);
}

/* Author: Dustin Yost
Sends a packet to the server indicating a private message
*/
void StateChatroom::sendMessage(const std::string &username, const std::string &message) {
	PacketString packet;
	packet.packetID = ID_SEND_ALL;

}
