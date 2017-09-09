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
		this->mpNetwork = network;
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

					UserID userId = this->getNextFreeID();

					//inputs that information into a pair of maps so the server has access to them
					this->mMapIDToAddress[userId] = systemAddress;
					this->mMapAddressToID[systemAddress] = userId;
					this->mMapIDToName[userId] = username;
					
					this->mpNetwork->sendTo(username, systemAddress, HIGH_PRIORITY, RELIABLE_ORDERED, 0, true);
					this->mpNetwork->sendTo(username, systemAddress, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
				}
				break;
			default:
				break;
		}
	}

	// Author: Dustin Yost
	// Performance: O(n^2)
	ServerPackets::UserID ServerPackets::getNextFreeID()
	{
		UserID nextID;
		// look for empty slots in the map
		for (nextID = 0; nextID < this->mMapIDToName.count; nextID++) {
			if (this->mMapIDToName.find(nextID) == this->mMapIDToName.end()) {
				// no entry for nextID - empty slot found
				break;
			}
			// nextID found in the map, find the next
		}
		// map has no empty slots, use the last value (which is the size of the map)
		return nextID;
	}

	// Author: Dustin Yost
	ServerPackets::UserName ServerPackets::getNameFromID(UserID id)
	{
		return this->mMapIDToName[id];
	}

}
