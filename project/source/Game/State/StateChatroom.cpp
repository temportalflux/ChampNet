/*
Dustin Yost 0984932
Jon Trusheim 0979380
EGP-405-02
Project 1
09/21/2017
We certify that this work is entirely our own.
The assessor of this project may reproduce this project and provide
copies to other academic staff, and/or communicate a copy of this project
to a plagiarism-checking service, which may retain a copy of
the project on its database.
*/

#include "Game\State\StateChatroom.h"

#include "Network\Framework.h"
#include "Network\Server.h"
#include "Network\Client.h"
#include "Game\Network\ServerPackets.h"
#include "Game\Network\ClientPackets.h"
#include "Game\State\StateLobby.h"

#include "Game\Packets\Packet.h"
#include "Network\Packets\PacketInfo.h"


#include <iostream>
#include <string>
#include <sstream>


StateChatroom::StateChatroom() : StateApplication() {
	mpNetworkFramework = NULL;
	mPacketInputQueue = std::deque<Network::PacketInfo*>();
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
void StateChatroom::handlePacket(Network::PacketInfo *info) {
	//this->pushMessage("Handling packet...");
	// Called from the updateNetwork loop
	// push a packet into the queue for later
	this->mPacketInputQueue.push_back(info);
}

void StateChatroom::updateGame() 
{

	// Author: Dustin Yost
	// Handle updates from the network packets
	while (this->mPacketInputQueue.size() > 0) {
		Network::PacketInfo *info = this->mPacketInputQueue.front();
		// Pass off the packet for handling in children classes
		this->doHandlePacket(info);
		// Pop the first packet (we are done with it)
		this->mPacketInputQueue.pop_front();
		delete info;
	}

	// Author: Jon Trusheim
	std::string latestLine;
	if (this->updateForInput(latestLine, false))
	{
		if (!chatCommands(latestLine)) {

			this->mData.display->textRecord.push_back(this->mData.network->networkInfo.username + ": " + latestLine);

			this->sendMessage(latestLine);
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

/*
Author: Jon Trusheim
Holds all chat room commands 
*/
bool StateChatroom::chatCommands(const std::string & latestLine)
{
	if (latestLine.at(0) == '/') //checks to make sure that the first character is a /
	{
		if (latestLine.find("/help") == 0)
		{
			this->mData.display->textRecord.push_back("type /pm to message a specfic user or type /clear to clear the screen\n You can also type /exit to leave the server\n(Try /taco, its yummy)");
		}
		else if (latestLine.find("/pm") == 0)
		{
			std::string username, message, temp;
			std::istringstream split(latestLine);
			int i = 0;
			while (split)
			{
				//used to get the /pm out of the message
				if (i == 0)
				{
					split >> temp;
					i++;
				}
				// splits the username off the message and stores it seperatly
				else if (i == 1)
				{
					split >> username;
					i++;
				}
				//starts of the message
				else if (i == 3)
				{
					split >> message;
				}
				//the rest of the message is added on one by one
				else
				{
					split >> temp;
					message.append(" " + temp);
				}
			}
			sendMessage(username, message);
		}
		// gives a command for users to choose when they would like to clear the screen
		else if (latestLine.find("/clear") == 0)
		{
			this->mData.display->textRecord.clear();
		}
		else if (latestLine.find("/taco") == 0)
		{
			this->sendMessage("NOM NOM NOM NOM NOM, TACO");
		}
		// lets you exit the current server and join a different one instead
		else if (latestLine.find("/exit") == 0)
		{
			this->mNext = new StateLobby();
		}
		else
		{
			// tells the user that it was not a valid command
			this->mData.display->textRecord.push_back("Not a valid command");
		}
		return true;
	}
	return false;
}

/* Author: Dustin Yost
Sends a packet to the server indicating a public message
*/
void StateChatroom::sendMessage(const std::string &message) {
	PacketStringLarge packet;
	packet.packetID = ID_CHAT_MESSAGE;
	packet.clientID = this->mData.network->clientID;
	size_t length = min(message.length(), PACKET_MAX_SIZE_CONTENT - 1);
	strncpy(packet.content, message.c_str(), length);
	packet.content[length] = '\0';
	this->sendToServer(&packet);
}

/* Author: Jon Trusheim
Sends a packet to the server indicating a private message
*/
void StateChatroom::sendMessage(const std::string &username, const std::string &message) 
{
	PacketChatMessage packet;
	packet.packetID = ID_PRIVATE_MESSAGE;
	packet.clientID = this->mData.network->clientID;
	strncpy(packet.message, message.c_str(), min(message.length(), PACKET_MAX_SIZE_CONTENT));
	strncpy(packet.username, username.c_str(), min(username.length(), PACKET_MAX_SIZE_CONTENT));
	this->sendToServer(&packet);
}
