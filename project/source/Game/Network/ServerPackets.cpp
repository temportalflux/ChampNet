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
#include "Game\Network\ServerPackets.h"

#include "Network\Network.h"
#include "Game\Packets\Packet.h"

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
				PacketString* packetUsername = (PacketString*)(info.data);
				//gets the username that the user inputed
				UserName username = packetUsername->content;

				printf("%s%s%s\n", "User ", username, " has joined");

				//gets the systemAdress of the 
				UserAddress systemAddress = *(info.address);

				UserID userId = this->getNextFreeID();

				//inputs that information into a pair of maps so the server has access to them
				this->mMapIDToAddress[userId] = systemAddress;
				this->mMapAddressToID[systemAddress] = userId;
				this->mMapIDToName[userId] = username;

				PacketString notifyNewUser = PacketString{ ID_NEW_CLIENT_JOINED };
				strncpy(notifyNewUser.content, username, 31);
				this->mpNetwork->sendTo(notifyNewUser, systemAddress, HIGH_PRIORITY, RELIABLE_ORDERED, 0, true);

				// Notify client of its ID
				this->mpNetwork->sendTo(PacketUInt{ ID_CLIENT_NUMBER, userId }, systemAddress, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);

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
	size_t count = this->mMapIDToName.size();
	// look for empty slots in the map
	for (nextID = 0; nextID < count; nextID++) {
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
