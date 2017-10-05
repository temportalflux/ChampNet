#ifndef _CHAMPNET_MAIN_H
#define _CHAMPNET_MAIN_H

#include <string>

namespace RakNet
{
	class RakPeerInterface;
	struct SystemAddress;
}
enum PacketPriority;
enum PacketReliability;

namespace ChampNet
{

	class PacketQueue;
	class Packet;

	class Network
	{

	private:
		// The interface used to connect to another computer
		RakNet::RakPeerInterface *mpPeerInterface;
		// All the packets to be processed
		PacketQueue* mpPackets;

	public:
		// Data types for Packets
		typedef char* Data;
		typedef unsigned int DataSize;

		Network();
		~Network();

		// Startup the server interface
		void initServer(const int port, const int maxClients);
		// Startup the client interface
		void initClient();
		// Connect the interface to its destination
		void connectToServer(const std::string &address, const int port);

		// Fetch the address the peer is bound to
		// TODO: Encapsulation Leek
		void queryAddress(RakNet::SystemAddress *address);

		// Return the IP string from the peer
		std::string getIP();

		// Returns true if the network interface (RakNet thread) is active
		bool isActive();

		// Shutdown the peer interface
		void disconnect();

		// Send packet data over RakNet
		// TODO: Encapsulation Leek
		void sendTo(Data data, DataSize size,
			RakNet::SystemAddress *address,
			PacketPriority *priority, PacketReliability *reliability,
			char channel, bool broadcast
		);

		// Handle sending struct packets to RakNet address
		// TODO: Encapsulation Leek
		template <typename T>
		void sendTo(T packet,
			RakNet::SystemAddress *address,
			PacketPriority *priority, PacketReliability *reliability,
			char channel, bool broadcast
		)
		{
			// Package up the packet
			Data data = (Data)(&packet);
			DataSize size = sizeof(packet);
			// Send via RakNet
			this->sendTo(data, size, address, priority, reliability, channel, broadcast);
		}

		// Handle sending struct packets to RakNet address
		// TODO: Encapsulation Leek
		template <typename T>
		void sendTo(T packet, RakNet::SystemAddress *address)
		{
			this->sendTo(packet, address, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
		}

		// Cache all incoming packets (should be run regularly)
		void fetchAllPackets();

		// Poll the next cached packet
		// Returns true if a packet was found;
		bool pollPackets(Packet *nextPacket);

	};

};

#endif // _CHAMPNET_MAIN_H