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
#include "Game\Network\ServerPackets.h"

#include "Network\Network.h"
#include "Game\Packets\Packet.h"
#include "Game\Network\MessageHandler.h"

// Author: Dustin Yost
ServerPackets::ServerPackets(MessageHandler *messageHandler) {
	mpMessageHandler = messageHandler;
}

// Author: Dustin Yost
ServerPackets::~ServerPackets() {
	mpMessageHandler = NULL;
}

void ServerPackets::subscribeTo(Network::Network* network) {
	// Author: Dustin Yost
	this->mpNetwork = network;
	network->addPacketHandler(ID_USERNAME, this);
	network->addPacketHandler(ID_CHAT_MESSAGE, this);
	network->addPacketHandler(ID_PRIVATE_MESSAGE, this);
	network->addPacketHandler(ID_CLIENT_LEFT, this);
}

void ServerPackets::handlePacketData(Network::PacketInfo *info) {
	this->mpMessageHandler->handlePacket(info);
}