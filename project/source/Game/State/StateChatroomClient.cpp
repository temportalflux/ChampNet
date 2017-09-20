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

#include "Game\State\StateChatroomClient.h"

#include "Game\Packets\Packet.h"
#include <sstream>
#include "Network\Framework.h"

using std::stringstream;

// AUTHOR: Dustin Yost
void StateChatroomClient::doHandlePacket(Network::PacketInfo info) {
	switch (info.getPacketType()) {
		case ID_CONNECTION_REQUEST_ACCEPTED: // Handle connections
			this->mAddressServer = info.address;
			{
				this->pushMessage("Connected.");

				PacketString packet;
				packet.packetID = GameMessages::ID_USERNAME;
				std::string username = this->mData.network->networkInfo.username;
				strncpy(packet.content, username.c_str(), username.length());

				this->sendPacket(packet);
			}
			break;
		case ID_NEW_CLIENT_JOINED: // Handle the message saying some client has joined
			{
				stringstream msg;
				msg << "User " << ((PacketString*)info.data)->content << " has joined.";
				this->pushMessage(msg.str());
			}
			break;
		case ID_CLIENT_NUMBER: // Handle incoming assignment to the user id
			this->pushMessage("Welcome to the server");
			break;
		default:
			break;
	}
}

void StateChatroomClient::render() {
	this->renderConsole();
}

void StateChatroomClient::sendPacket(Packet packet) {
	this->getFramework()->sendPacket(packet, this->mAddressServer);
}
