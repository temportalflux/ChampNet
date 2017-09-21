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
#pragma once

#include <string>

#include <RakNet\RakPeerInterface.h>
#include <RakNet\MessageIdentifiers.h>
#include <RakNet\PacketPriority.h>

namespace RakNet {
	struct Packet;
	//class RakPeerInterface;
};

// Author: Dustin Yost
// A container for all things network related
namespace Network {

	class PacketManager;
	class PacketHandler;

	/**
		A wrapper class for the RakNet interface
	*/
	class Network {

	protected:
		// The interface used to connect to another computer
		RakNet::RakPeerInterface *peerInterface;
		// The port this connection goes through
		unsigned int port;

		// The packet manager to handle processing of packets
		PacketManager* packetManager;

		// Abstract method to handle packets for clients and servers
		void handlePacket(RakNet::Packet *packet);

	public:

		/**
			Defines if this object is for a server or a client (NetworkServer or NetworkClient respectively)
		*/
		const bool isServer;

		// Base constructor, initializes the peer interface
		Network(bool isServer);
		Network() : Network(false) {}
		virtual ~Network();

		void queryAddress(RakNet::SystemAddress &address);
		std::string getIP();

		// Abstract method to startup the peer interface
		virtual void startup() = 0;
		// Abstract method to connect the interface to its destination
		virtual void connect() = 0;

		// Registers a packethandler to a specific packet identifier
		void addPacketHandler(unsigned char packet, PacketHandler *handler);

		// Handles updates for the network, specifically searching for new packets
		void update();

		void sendTo(char *data, unsigned int size, RakNet::SystemAddress *address,
			PacketPriority priority, PacketReliability reliability,
			char channel, bool broadcast
		) {
			// Send via RakNet
			this->peerInterface->Send(data, size, priority, reliability, channel, RakNet::AddressOrGUID(*address), broadcast);
		}

		// Handle sending struct packets to raknet address
		// TODO: Encapsulation Leek
		template <typename T>
		void sendTo(T packet, RakNet::SystemAddress address,
			PacketPriority priority, PacketReliability reliability,
			char channel, bool broadcast
		) {
			// Package up the packet
			char *data = (char*)(&packet);
			unsigned int size = sizeof(packet);
			// Send via RakNet
			this->peerInterface->Send(data, size, priority, reliability, channel, RakNet::AddressOrGUID(address), broadcast);
		}

		// packet - some bit aligned struct containing information to send to all connections
		template <typename T>
		void sendToAll(T &packet) {
			// Get total number of connections
			unsigned short connectionCount = this->peerInterface->NumberOfConnections();
			// Get the list of addresses
			RakNet::SystemAddress *addresses = new RakNet::SystemAddress[connectionCount];
			{
				// Populate the address list
				if (this->peerInterface->GetConnectionList(addresses, &connectionCount)) {
					// Loop through connections and send the passed packet as a notification of disconnection
					for (int connectionIndex = 0; connectionIndex < connectionCount; connectionIndex++) {
						this->sendTo(packet, addresses[connectionIndex], HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
					}
				}
			}
			// Delete the dynamic address list
			delete[]addresses;
			addresses = NULL;
		}

		// Handle disconnecting from the networked inferface
		// packet - some bit aligned struct containing information to send to all connections disconnecting from
		template <typename T>
		void disconnect(T &packet) {
			// Send message to all connections
			this->sendToAll(packet);
			// Disconnect the interface
			this->peerInterface->Shutdown(500, 0);
		}

	};

	class NetworkServer : public Network {

	private:
		// The max clients allowed on this server
		unsigned int maxClients;

	public:
		NetworkServer(unsigned int port, unsigned int maxClients);
		NetworkServer() : NetworkServer(0, 0) {}
		void startup();
		void connect();

	};

	class NetworkClient : public Network {

	private:
		// The address of the server
		std::string address;

	public:
		NetworkClient(std::string address, unsigned int port);
		NetworkClient() : NetworkClient("", 0) {}
		void startup();
		void connect();

	};

};
