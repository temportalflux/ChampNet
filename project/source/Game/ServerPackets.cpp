#include "Game\ServerPackets.h"

#include "Network\Network.h"
#include "Game\Packets\Packet.h"

namespace Game {

	ServerPackets::ServerPackets() {
		mMapNameToAddress = UserNameToAddress();
		mMapAddressToName = UserAddressToName();
	}

	ServerPackets::~ServerPackets() {

	}

	void ServerPackets::subscribeTo(Network::Network* network) {
		//network->addPacketHandler(ID_NEW_INCOMING_CONNECTION, this);
		//network->addPacketHandler(ID_REMOTE_NEW_INCOMING_CONNECTION, this);
		//network->addPacketHandler(ID_DISCONNECTION_NOTIFICATION, this);
		//network->addPacketHandler(ID_REMOTE_DISCONNECTION_NOTIFICATION, this);
		//network->addPacketHandler(ID_CONNECTION_LOST, this);
		//network->addPacketHandler(ID_REMOTE_CONNECTION_LOST, this);
		//network->addPacketHandler(ID_CLIENT_JOINED, this);
		network->addPacketHandler(ID_CLIENT_LEFT, this);
	}

	void ServerPackets::handlePacketData(Network::PacketInfo info) {
		/*
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
		*/
		switch (info.getPacketType()) {
			case ID_CLIENT_LEFT:
				{
					printf("%s\n", ((PacketString*)(info.data))->content);
				}
				break;
			default:
				break;
		}
	}

}
