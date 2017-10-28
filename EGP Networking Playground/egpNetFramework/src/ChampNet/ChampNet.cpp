#include "ChampNet\ChampNet.h"

#include <RakNet\RakPeerInterface.h>

#include "ChampNet\Packet.h"

namespace ChampNet
{

	Network::Network()
	{
		// instantiate the peer inferface
		mpPeerInterface = RakNet::RakPeerInterface::GetInstance();
		mpPackets = new PacketQueue();
	}

	Network::~Network()
	{
		// delete the peer interface
		RakNet::RakPeerInterface::DestroyInstance(mpPeerInterface);
		mpPeerInterface = NULL;
		
		delete mpPackets;
		mpPackets = NULL;

	}

	// Startup the server interface
	void Network::initServer(const int port, const int maxClients)
	{
		// Startup the server by reserving a port
		RakNet::SocketDescriptor sd = RakNet::SocketDescriptor(port, 0);
		this->mpPeerInterface->Startup(maxClients, &sd, 1);
		this->mpPeerInterface->SetMaximumIncomingConnections(maxClients);
	}

	// Startup the client interface
	void Network::initClient()
	{
		// Startup the client by starting on an empty socket
		RakNet::SocketDescriptor sd;
		this->mpPeerInterface->Startup(1, &sd, 1);
	}

	// Connect the interface to its destination
	void Network::connectToServer(const std::string &address, const int port)
	{
		// Connect to the server using the specified address and port
		this->mpPeerInterface->Connect(address.c_str(), port, 0, 0);
	}

	// Fetch the address the peer is bound to
	void Network::queryAddress(RakNet::SystemAddress *address)
	{
		address = &this->mpPeerInterface->GetMyBoundAddress();
	}

	// Return the IP string from the peer
	std::string Network::getIP()
	{
		return this->mpPeerInterface->GetLocalIP(0); // note: this can be finicky if there are multiple network addapters
	}

	// Returns true if the network interface (RakNet thread) is active
	bool Network::isActive()
	{
		return this->mpPeerInterface->IsActive() && this->mpPeerInterface->NumberOfConnections() > 0;
	}

	int Network::getPacketCount() const
	{
		return this->mpPackets->getCount();
	}

	// Shutdown the peer interface
	void Network::disconnect()
	{
		this->mpPeerInterface->Shutdown(500);
	}

	// Send packet data over RakNet
	void Network::sendTo(Data data, DataSize size,
		RakNet::SystemAddress *address,
		PacketPriority *priority, PacketReliability *reliability,
		char channel, bool broadcast)
	{
		this->mpPeerInterface->Send(data, size, *priority, *reliability, channel, *address, broadcast);
	}

	// Cache all incoming packets (should be run regularly)
	void Network::fetchAllPackets()
	{
		// RakNet Packet pointer
		RakNet::Packet *packet;
		// Wrapper Packet pointer
		ChampNet::Packet* pCurrentPacket;

		// Iterate over all packets in the interface
		for (packet = mpPeerInterface->Receive();
			packet;
			// DEALLOCATE PACKET WHEN FINISHED ITERATION
			mpPeerInterface->DeallocatePacket(packet), packet = mpPeerInterface->Receive())
		{
			// Process the packet

			// Copy out the addresss
			const char* address = packet->systemAddress.ToString();
			unsigned int addressLength = (unsigned int)std::strlen(address);

			// Send address, and packet data to copy, to a packet wrapper
			pCurrentPacket = new ChampNet::Packet(addressLength, address, packet->length, packet->data);

			// Save packet for processing later

			mpPackets->enqueue(pCurrentPacket);
		}
	}

	// Poll the next cached packet
	// Returns true if a packet was found;
	bool Network::pollPackets(Packet *&nextPacket)
	{
		mpPackets->dequeue(nextPacket);
		return nextPacket != NULL;
	}

}
