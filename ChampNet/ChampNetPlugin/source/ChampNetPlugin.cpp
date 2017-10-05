#include "ChampNetPlugin.h"
#include "ChampNet.h"

#include <RakNet\RakPeerInterface.h>
#include <string>

namespace ChampNetPlugin {

	//CHAMPNET_PLUGIN_SYMTAG ChampNet::Network *INSTANCE = 0;
	CHAMPNET_PLUGIN_SYMTAG RakNet::RakPeerInterface *gpPeerInterface = 0;
	CHAMPNET_PLUGIN_SYMTAG ChampNet::PacketQueue *gpPackets = 0;

	int Create() {
		/*
		if (INSTANCE == 0) {
			INSTANCE = new ChampNet::Network();
			return INSTANCE->init();
		}
		*/
		if (gpPeerInterface == 0) {
			gpPeerInterface = RakNet::RakPeerInterface::GetInstance();
			gpPackets = new ChampNet::PacketQueue();
			return true;
		}
		return 0;
	}

	int Destroy() {
		/*
		if (INSTANCE != 0) {
			delete INSTANCE;
			INSTANCE = 0;
			return 0;
		}
		*/
		if (gpPeerInterface != 0) {
			RakNet::RakPeerInterface::DestroyInstance(gpPeerInterface);
			gpPeerInterface = 0;
		}
		if (gpPackets != 0)
		{
			delete gpPackets;
			gpPackets = 0;
		}
		return 1;
	}

	// Startup the server interface
	int StartServer(int port, int maxClients) {
		if (gpPeerInterface != 0) {
			// Startup the server by reserving a port
			RakNet::SocketDescriptor sd = RakNet::SocketDescriptor(port, 0);
			gpPeerInterface->Startup(maxClients, &sd, 1);
			gpPeerInterface->SetMaximumIncomingConnections(maxClients);
			return 0;
		}
		return 1;
	}

	// Startup the client interface
	int StartClient()
	{
		if (gpPeerInterface != 0)
		{
			// Startup the client by starting on an empty socket
			RakNet::SocketDescriptor sd;
			gpPeerInterface->Startup(1, &sd, 1);
			return 0;
		}
		return 1;
	}

	// Connect the interface to its destination
	int ConnectToServer(std::string &address, int port)
	{
		if (gpPeerInterface != 0)
		{
			// Connect to the server using the specified address and port
			gpPeerInterface->Connect(address.c_str(), port, 0, 0);
			return 0;
		}
		return 1;
	}

	int FetchPackets()
	{

		// RakNet Packet pointer
		RakNet::Packet *packet;
		// Wrapper Packet pointer
		ChampNet::Packet* pCurrentPacket;

		// Iterate over all packets in the interface
		for (packet = gpPeerInterface->Receive();
			packet;
			// DEALLOCATE PACKET WHEN FINISHED ITERATION
			gpPeerInterface->DeallocatePacket(packet), packet = gpPeerInterface->Receive())
		{
			// Process the packet

			// Copy out the addresss
			char* address;
			packet->systemAddress.ToString(true, address);
			// Send address, and packet data to copy, to a packet wrapper
			pCurrentPacket = new ChampNet::Packet(std::strlen(address), address, packet->length, packet->data);

			// Save packet for processing later
			gpPackets->enqueue(pCurrentPacket);
		}

	}

	// TODO: Pass serialized data (all entries of gpPackets) back to caller

	/*
	int GetRandom() {
		return INSTANCE->getRandom();
	}
	*/

}
