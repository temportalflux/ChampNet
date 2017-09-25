/*
Dustin Yost 0984932
Jon Trusheim 0979380
EGP-405-02
Project 1
09/21/2017
We certify that this work is entirely our own.
The assessor of this project may reproduce this project and provide
copies to other academic staff, and/or communicate a copy of this project
to a plagiarism-checking service, which may retain a copy of
the project on its database.
*/
#include "Network\Net.h"

#include <RakNet\BitStream.h>
#include "Network\Packets\PacketInfo.h"
#include <RakNet\RakPeerInterface.h>
#include "Game\Network\MessageHandler.h"

// Base level networking / handling peer interfaces
namespace Network {

	Net::Net(MessageHandler *handler, bool server) : mIsServer(server) {
		this->mpPacketHandler = handler;
		// instantiate the peer inferface and packetmanager
		mpPeerInterface = RakNet::RakPeerInterface::GetInstance();
		//this->packetManager = new PacketManager();
	}

	Net::~Net() {
		this->mpPacketHandler = NULL; // not owned by us
		// delete the peer interface and packetmanager
		RakNet::RakPeerInterface::DestroyInstance(mpPeerInterface);
		//delete packetManager;
	}

	void Net::queryAddress(RakNet::SystemAddress &address) {
		address = this->mpPeerInterface->GetMyBoundAddress();
	}

	std::string Net::getIP() {
		return this->mpPeerInterface->GetLocalIP(0); // note: this can be finicky if there are multiple network addapters
	}

	void Net::update() {
		// Packet pointer
		RakNet::Packet *packet;
		// Iterate over all packets in the interface
		for (packet = this->mpPeerInterface->Receive();
			packet;
			this->mpPeerInterface->DeallocatePacket(packet), packet = this->mpPeerInterface->Receive())
		{
			// Handle the packets accordingly
			this->handlePacket(packet);
		}
	}

	void Net::handlePacket(RakNet::Packet *packet) {
		unsigned char id = packet->data[0];
		if (mpPacketHandler != NULL) {
			// Create encapsulation struct containing packet info
			PacketInfo *info = new PacketInfo;
			// this, &(packet->systemAddress), packet->data, packet->length
			info->network = NULL; // TODO: this;
			info->address = packet->systemAddress;
			info->length = packet->length;

			unsigned char *test = new unsigned char[packet->length]; // delete in PacketInfo descructor
			for (int i = 0; i < info->length; i++) {
				test[i] = packet->data[i];
			}
			info->data = test;

			// Pass off to be handled by the handler
			mpPacketHandler->handlePacket(info);
		}
	}

	void Net::disconnect() {
		this->mpPeerInterface->Shutdown(500);
	}

	/*
	// ~~~~~~~~~~ SERVER ~~~~~~~~~~

	NetworkServer::NetworkServer(unsigned int port, unsigned int maxClients)
		: Network(true) {
		this->port = port;
		this->maxClients = maxClients;
	}

	void NetworkServer::startup() {
		// Startup the server by reserving a port
		RakNet::SocketDescriptor sd = RakNet::SocketDescriptor(this->port, 0);
		this->peerInterface->Startup(this->maxClients, &sd, 1);
	}

	void NetworkServer::connect() {
		this->peerInterface->SetMaximumIncomingConnections(this->maxClients);
	}

	// ~~~~~~~~~~ CLIENT ~~~~~~~~~~

	NetworkClient::NetworkClient(std::string address, unsigned int port) : Network(false) {
		this->address = address;
		this->port = port;
	}

	void NetworkClient::startup() {
		// Startup the client by starting on an empty socket
		RakNet::SocketDescriptor sd;
		this->peerInterface->Startup(1, &sd, 1);
	}

	void NetworkClient::connect() {
		// Connect to the server using the specified address and port
		this->peerInterface->Connect(address.c_str(), this->port, 0, 0);
	}
	*/

}
