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

void StateChatroomServer::doHandlePacket(Network::PacketInfo *info) {
	unsigned int id = info->getPacketType();
	switch (id) {
		case ID_USERNAME: // Handle connections
			{
				PacketString* packetUsername = (PacketString*)(info->data);
				//gets the username that the user inputed
				StateNetwork::UserName username = packetUsername->content;

				//this->pushMessage(std::string("User ") + username + " joined.");
				this->pushMessage(username);

				//gets the systemAdress of the 
				StateNetwork::UserAddress systemAddress = info->address;

				StateNetwork::UserID userId = this->mData.network->getNextFreeID();

				//inputs that information into a pair of maps so the server has access to them
				this->mData.network->mMapIDToAddress[userId] = systemAddress;
				this->mData.network->mMapAddressToID[systemAddress] = userId;
				this->mData.network->mMapIDToName[userId] = username;

				PacketString notifyNewUser;
				notifyNewUser.packetID = ID_NEW_CLIENT_JOINED;
				strncpy(notifyNewUser.content, username, 31);
				this->sendTo(notifyNewUser, systemAddress, true);

			}
			break;
		case ID_PRIVATE_MESSAGE:
			info->address;
			break;
		default:
			this->pushMessage("Recieved message id " + id);
			break;
	}
}

void StateChatroomServer::render() {
	this->renderConsole();
}

void StateChatroomServer::sendTo(PacketString packet) 
{
	// Never send from server to self, pack into PacketInfo and use
}

void StateChatroomServer::sendTo(PacketChatMessage packet)
{
	char *data = (char*)(&packet);
	unsigned int size = sizeof(packet);
	//this->getFramework()->sendTo(data, size, /*RakNet stuff*/, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
}

void StateChatroomServer::sendTo(PacketString packet, RakNet::SystemAddress *address, bool broadcast) {
	char *data = (char*)(&packet);
	unsigned int size = sizeof(packet);
	this->getFramework()->sendTo(data, size, address, HIGH_PRIORITY, RELIABLE_ORDERED, 0, broadcast);
}

void StateChatroomServer::sendTo(PacketUInt packet, RakNet::SystemAddress *address, bool broadcast) {
	char *data = (char*)(&packet);
	unsigned int size = sizeof(packet);
	this->getFramework()->sendTo(data, size, address, HIGH_PRIORITY, RELIABLE_ORDERED, 0, broadcast);
}
