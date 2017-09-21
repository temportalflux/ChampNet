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
#include "Game\Network\ClientPackets.h"

#include "Network\Network.h"
#include "Game\Packets\Packet.h"
#include "Game\Network\MessageHandler.h"

#include <iostream>
#include <string>

// AUTHOR: Dustin Yost
ClientPackets::ClientPackets(MessageHandler *messageHandler) {
	mpMessageHandler = messageHandler;
}

// AUTHOR: Dustin Yost
ClientPackets::~ClientPackets() {
	mpNetwork = NULL; // not owned by us
	mpMessageHandler = NULL;
}

// Author: Dustin Yost
void ClientPackets::subscribeTo(Network::Network* network) {
	this->mpNetwork = network;
	network->addPacketHandler(ID_CONNECTION_REQUEST_ACCEPTED, this);
	network->addPacketHandler(ID_NEW_CLIENT_JOINED, this);
	network->addPacketHandler(ID_CLIENT_NUMBER, this);
	network->addPacketHandler(ID_SEND_ALL, this);
	network->addPacketHandler(ID_PRIVATE_MESSAGE, this);
	network->addPacketHandler(ID_CHAT_MESSAGE, this);
	network->addPacketHandler(ID_CLIENT_LEFT, this);
}

// AUTHOR: Dustin Yost
void ClientPackets::handlePacketData(Network::PacketInfo *info) {
	switch (info->getPacketType()) {
		// AUTHOR: Dustin Yost
		case ID_CONNECTION_REQUEST_ACCEPTED: // Handle connections
			{
				//this->mAddressServer = info.address;

				/*
				// Construct response packet
				PacketString packetUsername = PacketString{ ID_USERNAME };

				// Prompt for username
				printf("Enter Username (30 characters): ");
				fgets(packetUsername.content, 31, stdin); // clean up input
														  // Remove newline from string
				for (int i = 0; i < strlen(packetUsername.content); i++)
					if (packetUsername.content[i] == '\n') {
						packetUsername.content[i] = '\0';
						break;
					}

				// Send username to server
				this->mpNetwork->sendTo(packetUsername, *(this->mAddressServer), HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
				//*/

			}
			break;
			// AUTHOR: Dustin Yost
		case ID_CLIENT_NUMBER: // Handle incoming assignment to the user id
			mID = ((PacketUInt*)info->data)->clientID;
			break;
		default:
			break;
	}

	this->mpMessageHandler->handlePacket(info);

}

void ClientPackets::onExit() {
	PacketNotification packet = { ID_CLIENT_LEFT };
	this->mpNetwork->disconnect(packet);
}
