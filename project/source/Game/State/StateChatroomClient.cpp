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

void StateChatroomClient::updateGame() {
	StateChatroom::updateGame();

	if (this->mEscape) {
		PacketUInt packet;
		packet.packetID = ID_CLIENT_LEFT;
		packet.clientID = this->mData.network->clientID;
		this->sendToServer(&packet);

		this->mRunning = false;
	}

}

// AUTHOR: Dustin Yost
void StateChatroomClient::doHandlePacket(Network::PacketInfo *info) {
	unsigned char id = info->getPacketType();
	switch (id) {
		case ID_CONNECTION_REQUEST_ACCEPTED: // Handle connections
			this->mAddressServer = info->address;
			{
				this->pushMessage("Connected.");

				std::string username = this->mData.network->networkInfo.username;

				PacketUsername packet;
				packet.packetID = GameMessages::ID_USERNAME;
				writeToCharData(packet.address, this->getFramework()->getIP(), PACKET_SIZE_IPV4);
				writeToCharData(packet.content, username, PACKET_USERNAME_SIZE_CONTENT);

				this->sendToServer(&packet);
			}
			break;
		case ID_NEW_CLIENT_JOINED: // Handle the message saying some client has joined
			{
				stringstream msg;
				msg << "User " << ((PacketString*)info->data)->content << " has joined.";
				this->pushMessage(msg.str());
			}
			break;
		case ID_CLIENT_NUMBER: // Handle incoming assignment to the user id
			this->mData.network->clientID = ((PacketUInt*)info->data)->clientID;
			this->pushMessage("Welcome to the server");
			break;

		//Author: Jon Trusheim
		case ID_CHAT_MESSAGE:  //Handle incomeing messages that are ment for all users
			{
				PacketStringLarge *packet = ((PacketStringLarge*)info->data);
				std::string message = std::string(packet->content);
				this->pushMessage(message);
				break;
			}
		//Author: Jon Trusheim
		case ID_PRIVATE_MESSAGE: //Handle incoming private message
			{
				PacketChatMessage *packet = (PacketChatMessage*)info->data;

				stringstream msg;
				msg << "PM | " << packet->username << ": " << packet->message;
				this->pushMessage(msg.str());

				break;
			}
		case ID_CLIENT_LEFT:
		{
			this->pushMessage(std::string(((PacketString*)info->data)->content));
			break;
		}
		default:
			this->pushMessage("Recieved message id " + id);
			break;
	}
}

void StateChatroomClient::render() {
	this->renderConsole();
}

void StateChatroomClient::sendToServer(PacketString *packet) {
	PacketString obj = *packet;
	char *data = (char*)(&obj);
	unsigned int size = sizeof(obj);
	this->getFramework()->sendTo(data, size, this->mAddressServer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
}

void StateChatroomClient::sendToServer(PacketChatMessage *packet) {
	PacketChatMessage obj = *packet;
	char *data = (char*)(&obj);
	unsigned int size = sizeof(obj);
	this->getFramework()->sendTo(data, size, this->mAddressServer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
}

void StateChatroomClient::sendToServer(PacketStringLarge *packet) {
	PacketStringLarge obj = *packet;
	char *data = (char*)(&obj);
	unsigned int size = sizeof(obj);
	this->getFramework()->sendTo(data, size, this->mAddressServer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
}

void StateChatroomClient::sendToServer(PacketUsername *packet) {
	PacketUsername obj = *packet;
	char *data = (char*)(&obj);
	unsigned int size = sizeof(obj);
	this->getFramework()->sendTo(data, size, this->mAddressServer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
}

void StateChatroomClient::sendToServer(PacketUInt *packet) {
	PacketUInt obj = *packet;
	char *data = (char*)(&obj);
	unsigned int size = sizeof(obj);
	this->getFramework()->sendTo(data, size, this->mAddressServer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
}
