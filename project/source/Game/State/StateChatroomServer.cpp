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

void StateChatroomServer::doHandlePacket(Network::PacketInfo info) {
	switch (info.getPacketType()) {
		case ID_USERNAME: // Handle connections
			this->pushMessage("User joined.");
			break;
		default:
			break;
	}
}

void StateChatroomServer::render() {
	this->renderConsole();
}

void StateChatroomServer::sendPacket(Packet packet) {
	Network::PacketInfo i;
	i.address = NULL;
	i.network = NULL;
	i.data = (unsigned char*)(&packet);
	i.length = sizeof(packet);
	this->handlePacket(i);
}

