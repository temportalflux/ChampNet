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

#include "Game\State\StateChatroomServer.h"
#include "Game\Packets\Packet.h"
#include "Network\Packets\PacketInfo.h"
#include "Network\Framework.h"
#include <RakNet\RakNetTypes.h>

void StateChatroomServer::doHandlePacket(Network::PacketInfo *info) {
	unsigned int id = info->getPacketType();
	switch (id) {
		case ID_USERNAME: // Handle connections
			{
				PacketUsername* packetUsername = (PacketUsername*)(info->data);
				//gets the username that the user inputed
				StateNetwork::UserName username = std::string(packetUsername->content);
				std::string addressIPv4 = std::string(packetUsername->address);
				StateNetwork::UserAddress systemAddress = info->address;

				std::stringstream msg;
				msg << "User " << username << " has joined at IP " << addressIPv4 << ".";
				this->pushMessage(msg.str());

				StateNetwork::UserID userId = this->mData.network->getNextFreeID();

				//inputs that information into a pair of maps so the server has access to them
				//this->mData.network->mMapIDToAddress[userId] = systemAddress;
				//this->mData.network->mMapAddressToID[systemAddress] = userId;
				//this->mData.network->mMapIDToName[userId] = username;
				//this->mData.network->mMapNameToID[username] = userId;
				this->mData.network->mMapIDToAddress[userId] = systemAddress;
				this->mData.network->mMapIDToName[userId] = username;

				PacketString notifyNewUser;
				notifyNewUser.packetID = ID_NEW_CLIENT_JOINED;
				writeToCharData(notifyNewUser.content, username, PACKET_MAX_SIZE_CONTENT);
				this->sendTo(&notifyNewUser, systemAddress, true);

				PacketUInt packetID = PacketUInt{ ID_CLIENT_NUMBER, userId };
				this->sendTo(&packetID, systemAddress);

			}
			break;
		case ID_CHAT_MESSAGE:
			{
				PacketStringLarge *packet = ((PacketStringLarge*)info->data);
				StateNetwork::UserID sourceID = packet->clientID;// this->mData.network->mMapAddressToID[info->address];
				StateNetwork::UserName sourceName = this->mData.network->mMapIDToName[sourceID];
				std::string content = std::string(packet->content);

				std::stringstream msg;
				msg << sourceName << ": " << content;
				std::string msgContent = msg.str();
				this->pushMessage(msgContent);

				size_t length = min(msgContent.length(), PACKET_MAX_SIZE_TEXT - 1);
				strncpy(packet->content, msgContent.c_str(), length);
				packet->content[length] = '\0';
				
				this->sendTo(packet, info->address, true);

			}
			break;
		case ID_PRIVATE_MESSAGE:
			{
				PacketChatMessage *packet = ((PacketChatMessage*)info->data);

				StateNetwork::UserID targetID = this->mData.network->getIDFromName(packet->username);
				//StateNetwork::UserID targetID = this->mData.network->mMapNameToID[packet->username];
				StateNetwork::UserID sourceID = packet->clientID;// this->mData.network->mMapAddressToID[info->address];
				StateNetwork::UserAddress targetAddress = this->mData.network->mMapIDToAddress[targetID];
				StateNetwork::UserName sourceName = this->mData.network->mMapIDToName[sourceID];

				writeToCharData(packet->username, sourceName, PACKET_MAX_SIZE_CONTENT);

				this->sendTo(packet, targetAddress);

			}
			break;
		case ID_CLIENT_LEFT:
			{
				PacketUInt *packet = (PacketUInt*)info->data;

				std::stringstream msg;
				//StateNetwork::UserID id = this->mData.network->mMapAddressToID[info->address];
				StateNetwork::UserID id = packet->clientID;
				StateNetwork::UserName username = this->mData.network->mMapIDToName[id];
				msg << "User " << std::string(username) << " has left.";
				this->pushMessage(msg.str());

				this->mData.network->mMapIDToAddress[id] = NULL;
				this->mData.network->mMapIDToName[id] = "";

				break;
			}
		default:
			this->pushMessage("Recieved message id " + id);
			break;
	}
}

void StateChatroomServer::render() {
	this->renderConsole();
}

void StateChatroomServer::sendToServer(PacketString *packet) {
	this->sendTo(packet, NULL, true);
}

void StateChatroomServer::sendToServer(PacketChatMessage *packet) {
	StateNetwork::UserID targetID = this->mData.network->getIDFromName(packet->username);
	StateNetwork::UserAddress targetAddress = this->mData.network->mMapIDToAddress[targetID];
	this->sendTo(packet, targetAddress, true);
}

void StateChatroomServer::sendToServer(PacketStringLarge *packet) {
	RakNet::SystemAddress me;
	this->getFramework()->queryAddress(me);
	this->sendTo(packet, &me, true);
}

void StateChatroomServer::sendToServer(PacketUsername *packet) {
	// STUB: Unused
}

void StateChatroomServer::sendTo(PacketString *packet, RakNet::SystemAddress *address, bool broadcast)
{
	PacketString obj = *packet;
	char *data = (char*)(&obj);
	unsigned int size = sizeof(obj);
	this->getFramework()->sendTo(data, size, address, HIGH_PRIORITY, RELIABLE_ORDERED, 0, broadcast);
}

void StateChatroomServer::sendTo(PacketUInt *packet, RakNet::SystemAddress *address, bool broadcast)
{
	PacketUInt obj = *packet;
	char *data = (char*)(&obj);
	unsigned int size = sizeof(obj);
	this->getFramework()->sendTo(data, size, address, HIGH_PRIORITY, RELIABLE_ORDERED, 0, broadcast);
}

void StateChatroomServer::sendTo(PacketChatMessage *packet, RakNet::SystemAddress *address, bool broadcast)
{
	PacketChatMessage obj = *packet;
	char *data = (char*)(&obj);
	unsigned int size = sizeof(obj);
	this->getFramework()->sendTo(data, size, address, HIGH_PRIORITY, RELIABLE_ORDERED, 0, broadcast);
}

void StateChatroomServer::sendTo(PacketStringLarge *packet, RakNet::SystemAddress *address, bool broadcast)
{
	PacketStringLarge obj = *packet;
	char *data = (char*)(&obj);
	unsigned int size = sizeof(obj);
	this->getFramework()->sendTo(data, size, address, HIGH_PRIORITY, RELIABLE_ORDERED, 0, broadcast);
}

void StateChatroomServer::sendTo(PacketStringLargeUsername *packet, RakNet::SystemAddress *address, bool broadcast)
{
	PacketStringLargeUsername obj = *packet;
	char *data = (char*)(&obj);
	unsigned int size = sizeof(obj);
	this->getFramework()->sendTo(data, size, address, HIGH_PRIORITY, RELIABLE_ORDERED, 0, broadcast);
}
