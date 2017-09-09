/*
	Dustin Yost 0984932
	EGP-405-02
	Project 1
	09/21/2017
	We certify that this work is entirely our own.
	The assessor of this project may reproduce this project and provide
	copies to other academic staff, and/or communicate a copy of this project
	to a plagiarism-checking service, which may retain a copy of
	the project on its database.
*/
#include "Game\ServerPackets.h"

#include "Network\Network.h"
#include "Game\Packets\Packet.h"

namespace Game {

	// Author: Dustin Yost
	ServerPackets::ServerPackets() {
		mMapIDToAddress = UserIDToAddress();
		mMapAddressToID = UserAddressToID();
		mMapIDToName = UserIDToName();
	}

	// Author: Dustin Yost
	ServerPackets::~ServerPackets() {

	}

	void ServerPackets::subscribeTo(Network::Network* network) {
		// Author: Dustin Yost
		network->addPacketHandler(ID_CLIENT_LEFT, this);
		network->addPacketHandler(ID_USERNAME, this);
	}

	void ServerPackets::handlePacketData(Network::PacketInfo info) {
		switch (info.getPacketType()) {
			// Author: Dustin Yost
			case ID_CLIENT_LEFT:
				{
					printf("%s\n", ((PacketString*)(info.data))->content);
				}
				break;
			//Author: Jon T
			case ID_USERNAME:
				{
					//gets the username that the user inputed
					UserName username = ((PacketString*)(info.data))->content;
					//gets the systemAdress of the 
					UserAddress systemAddress = (info.address);
					//inputs that information into a pair of maps so the server has access to them
					this->mMapIDToAddress[username] = systemAddress;
					this->mMapAddressToID[systemAddress] = username;
				}
				break;
			default:
				break;
		}
	}

}
