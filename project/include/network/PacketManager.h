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
#pragma once

#include <map>

#include <RakNet\RakNetTypes.h>  // MessageID

// Author: Dustin Yost
namespace Network {

	class Network;

	struct PacketInfo {

		// The network which recieved the packet
		Network *network;
		// The address the packet was sent from
		RakNet::SystemAddress address; // TODO: Encapsulation Leak
		// The packet data
		unsigned char *data;
		// The bit length of the packet data
		unsigned int length;

		unsigned int getPacketType() { return this->data[0]; }

	};

	class PacketHandler {
	
	public:

		virtual void subscribeTo(Network *network) = 0;

		// Handle incoming packets
		virtual void handlePacketData(PacketInfo info) = 0;

		virtual void onExit() = 0;

	};

	class PacketManager {
	
	public:
		typedef unsigned char PacketID;
		typedef PacketHandler* Value;
		typedef std::multimap<PacketID, Value> Map;
		typedef std::pair<PacketID, Value> Pair;

	private:
		// A map of packet IDs to their handler values
		Map packetHandlers;

	public:
		PacketManager();

		// Alerts the manager of some packet with the id of packetID and the type of structID
		// Use Network::BASE_PACKET_ENUM as the first value for the implementation enum of packetID
		// Use typeid(T) of the packet struct for the class
		void registerHandler(PacketID packetID, Value handler);

		// Returns the first handler for the ID, null if none exists
		Value getHandlerFor(PacketID id);

	};

}
