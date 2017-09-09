#include "Game\ClientPackets.h"

#include "Network\Network.h"
#include "Game\Packets\Packet.h"

#include <iostream>

namespace Game {

	ClientPackets::ClientPackets() {

	}

	ClientPackets::~ClientPackets() {
		mpNetwork = NULL; // not owned by us
	}

	void ClientPackets::subscribeTo(Network::Network* network) {
		this->mpNetwork = network;
		network->addPacketHandler(ID_CONNECTION_REQUEST_ACCEPTED, this);
		//network->addPacketHandler(ID_NO_FREE_INCOMING_CONNECTIONS, this);
		//network->addPacketHandler(ID_DISCONNECTION_NOTIFICATION, this);
		//network->addPacketHandler(ID_CONNECTION_LOST, this);
		//network->addPacketHandler(ID_CLIENT_JOINED_CONFIRM, this);
	}

	void ClientPackets::handlePacketData(Network::PacketInfo info) {
		/*
		switch (info.data[0])
		{
			// the request to join a server has been granted
		case ID_CONNECTION_REQUEST_ACCEPTED:
		{
			//printf("> Our connection request has been accepted.\n");

			PacketMessage packetMsg = PacketMessage();
			packetMsg.typeId = (unsigned char)ID_CLIENT_JOINED;
			packetMsg.message = "Hello server";
			info.network->sendTo(packetMsg, info.address, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);

		}
		break;
		// tried to join, but the server was full
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			printf("> The server is full.\n");
			break;
			// client disconnected
		case ID_DISCONNECTION_NOTIFICATION:
			printf("> We have been disconnected.\n");
			break;
			// client lost connection
		case ID_CONNECTION_LOST:
			printf("> Connection lost.\n");
			break;
		case ID_CLIENT_JOINED_CONFIRM:
		{
			//printf("> Joined confirmation.\n");
			printf("%s\n", ((PacketMessage*)(info.data))->message);
			break;
		}
		default:
			printf("> Message with identifier %i has arrived.\n", info.data[0]);
			break;
		}
		*/
		switch (info.getPacketType()) {
			case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					// AUTHOR: Dustin Yost
					this->mAddressServer = info.address;

					// Construct response packet
					PacketString packetUsername = PacketString{ ID_USERNAME };

					// Prompt for username
					printf("Enter Username (30 characters): ");
					fgets(packetUsername.content, 31, stdin); // clean up input
					
					// Send username to server
					this->mpNetwork->sendTo(packetUsername, this->mAddressServer, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
					
				}
				break;
			default:
				break;
		}
	}

	void ClientPackets::onExit() {
		this->mpNetwork->disconnect(
			PacketString{ (unsigned char)GameMessages::ID_CLIENT_LEFT, "Client is leaving." }
		);
	}

}
