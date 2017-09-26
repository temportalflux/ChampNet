#pragma once

#include <RakNet\RakNetTypes.h>
#include <RakNet\PacketPriority.h>
#include <string>

namespace RakNet {
	struct Packet;
	class RakPeerInterface;
}

class MessageHandler;

/* Author: Dustin Yost
	Handles management of RakNet packets
*/
class Net {

protected:
	// If this network is a server for a server/client setup
	bool mIsServer;
	// The interface used to connect to another computer
	RakNet::RakPeerInterface *mpPeerInterface;
	// The handler for all incoming packets
	MessageHandler *mpPacketHandler;

	// Forwarding method to handle packets
	void handlePacket(RakNet::Packet *packet);

public:
	// Data types for Packets
	typedef char* Data;
	typedef unsigned int DataSize;

	// Base constructor, initializes the peer interface
	Net(MessageHandler* handler, bool isServer);
	Net(MessageHandler* handler) : Net(handler, false) {}
	virtual ~Net();

	// Startup the server interface
	void initServer(int port, int maxClients);
	// Startup the client interface
	void initClient();
	// Connect the interface to its destination
	void connectToServer(std::string &address, int port);

	void setHandler(MessageHandler *handler);

	// Fetch the address the peer is bound to
	// TODO: Encapsulation Leek
	void queryAddress(RakNet::SystemAddress &address);

	// Return the IP string from the peer
	std::string getIP();

	// Handles updates for the network, specifically searching for new packets
	void update();

	// Send packet data over RakNet
	// TODO: Encapsulation Leek
	void sendTo(Data data, DataSize size,
		RakNet::SystemAddress address,
		PacketPriority priority, PacketReliability reliability,
		char channel, bool broadcast
	);

	// Handle sending struct packets to RakNet address
	// TODO: Encapsulation Leek
	template <typename T>
	void sendTo(T packet,
		RakNet::SystemAddress address,
		PacketPriority priority, PacketReliability reliability,
		char channel, bool broadcast
	) {
		// Package up the packet
		Data data = (Data)(&packet);
		DataSize size = sizeof(packet);
		// Send via RakNet
		this->sendTo(data, size, address, priority, reliability, channel, broadcast);
	}

	// Shutdown the peer interface
	void disconnect();

};
