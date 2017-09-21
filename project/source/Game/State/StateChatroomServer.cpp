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
			this->pushMessage("User joined.");
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

}

void StateChatroomServer::sendTo(PacketChatMessage packet)
{
	char *data = (char*)(&packet);
	unsigned int size = sizeof(packet);
	this->getFramework()->sendTo(data, size, /*RakNet stuff*/, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
}
