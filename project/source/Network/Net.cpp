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
Net::Net(MessageHandler *handler, bool server) : mIsServer(server) {
	this->mpPacketHandler = handler;
	// instantiate the peer inferface and packetmanager
	mpPeerInterface = RakNet::RakPeerInterface::GetInstance();
}

Net::~Net() {
	// not owned by us
	this->mpPacketHandler = NULL; 
	 // delete the peer interface
	RakNet::RakPeerInterface::DestroyInstance(mpPeerInterface);
}

void Net::initServer(int port, int maxClients) {
	// Startup the server by reserving a port
	RakNet::SocketDescriptor sd = RakNet::SocketDescriptor(port, 0);
	this->mpPeerInterface->Startup(maxClients, &sd, 1);
	this->mpPeerInterface->SetMaximumIncomingConnections(maxClients);
}

void Net::initClient() {
	// Startup the client by starting on an empty socket
	RakNet::SocketDescriptor sd;
	this->mpPeerInterface->Startup(1, &sd, 1);
}

void Net::connectToServer(std::string &address, int port)
{
	// Connect to the server using the specified address and port
	this->mpPeerInterface->Connect(address.c_str(), port, 0, 0);
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
		// DEALLOCATE PACKET WHEN FINISHED ITERATION
		this->mpPeerInterface->DeallocatePacket(packet), packet = this->mpPeerInterface->Receive())
	{
		// Handle the packets accordingly
		this->handlePacket(packet);
	}
}

// Send packet data over RakNet
void Net::sendTo(Data data, DataSize size,
	RakNet::SystemAddress address,
	PacketPriority priority, PacketReliability reliability,
	char channel, bool broadcast)
{
	this->mpPeerInterface->Send(data, size, priority, reliability, channel, address, broadcast);
}

// Forwarding method to handle packets
void Net::handlePacket(RakNet::Packet *packet) {
	unsigned char id = packet->data[0];
	if (mpPacketHandler != NULL) {
		// Create encapsulation struct containing packet info
		Network::PacketInfo *info = new Network::PacketInfo;
		// this, &(packet->systemAddress), packet->data, packet->length
		info->network = NULL; // TODO: this;
		info->address = packet->systemAddress;
		info->length = packet->length;

		unsigned char *test = new unsigned char[packet->length]; // delete in PacketInfo descructor
		for (unsigned int i = 0; i < info->length; i++) {
			test[i] = packet->data[i];
		}
		info->data = test;

		// Pass off to be handled by the handler
		mpPacketHandler->handlePacket(info);

		delete info;
	}
}

// Shutdown the peer interface
void Net::disconnect() {
	this->mpPeerInterface->Shutdown(500);
}
