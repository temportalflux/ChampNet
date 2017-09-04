/*
	Dustin Yost 0984932
	EGP-405-02
	Lab1
	09/03/2017
	We certify that this work is entirely our own.
	The assessor of this project may reproduce this project and provide
	copies to other academic staff, and/or communicate a copy of this project
	to a plagiarism-checking service, which may retain a copy of
	the project on its database.
*/
#include "PacketHandlerImpl.h"

#include "packets\Packet.h"

namespace NetworkImpl {

	PacketHandler::PacketHandler(bool isServer) : Network::PacketHandler() {
		this->isServer = isServer;
	}

	void PacketHandler::subscribeTo(Network::Network *network) {
		if (this->isServer) {
			network->addPacketHandler(ID_NEW_INCOMING_CONNECTION, this);
			network->addPacketHandler(ID_REMOTE_NEW_INCOMING_CONNECTION, this);
			network->addPacketHandler(ID_DISCONNECTION_NOTIFICATION, this);
			network->addPacketHandler(ID_REMOTE_DISCONNECTION_NOTIFICATION, this);
			network->addPacketHandler(ID_CONNECTION_LOST, this);
			network->addPacketHandler(ID_REMOTE_CONNECTION_LOST, this);
			network->addPacketHandler(ID_CLIENT_JOINED, this);
			network->addPacketHandler(ID_CLIENT_LEFT, this);
		}
		else {
			network->addPacketHandler(ID_CONNECTION_REQUEST_ACCEPTED, this);
			network->addPacketHandler(ID_NO_FREE_INCOMING_CONNECTIONS, this);
			network->addPacketHandler(ID_DISCONNECTION_NOTIFICATION, this);
			network->addPacketHandler(ID_CONNECTION_LOST, this);
			network->addPacketHandler(ID_CLIENT_JOINED_CONFIRM, this);
		}
	}

	void PacketHandler::handlePacketData(Network::PacketInfo info) {
		if (this->isServer) {
			this->handlePacketServer(info);
		}
		else {
			this->handlePacketClient(info);
		}
	}

	void PacketHandler::handlePacketServer(Network::PacketInfo info) {
		switch (info.data[0])
		{
			// some client connection is approaching
		case ID_NEW_INCOMING_CONNECTION:
			printf("> A connection is incoming.\n");
			break;
			// some client has connected
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			printf("> Another client has connected.\n");
			break;
			// a specific client disconnected
		case ID_DISCONNECTION_NOTIFICATION:
			printf("> A client has disconnected.\n");
			break;
			// some client has disconnected
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			printf("> Another client has disconnected.\n");
			break;
			// a specific client went missing
		case ID_CONNECTION_LOST:
			printf("> A client lost the connection.\n");
			break;
			// some client went missing
		case ID_REMOTE_CONNECTION_LOST:
			printf("> Another client has lost the connection.\n");
			break;
		case ID_CLIENT_JOINED:
		{
			//printf("> client is joining\n");

			printf("%s\n", ((PacketMessage*)(info.data))->message);

			info.network->sendTo(
				PacketMessage{ (unsigned char)ID_CLIENT_JOINED_CONFIRM , "Welcome to the server" },
				info.address, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false
			);

			break;
		}
		case ID_CLIENT_LEFT:
		{
			printf("%s\n", ((PacketMessage*)(info.data))->message);
			break;
		}
		default:
			printf("> Message with identifier %i has arrived.\n", info.data[0]);
			break;
		}
	}

	void PacketHandler::handlePacketClient(Network::PacketInfo info) {
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
	}

}
