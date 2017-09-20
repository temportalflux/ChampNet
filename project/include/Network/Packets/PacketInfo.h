#pragma once

namespace RakNet {
	struct SystemAddress;
}

// Author: Dustin Yost
namespace Network {

	class Network;

	struct PacketInfo {

		// The network which recieved the packet
		Network *network;
		// The address the packet was sent from
		RakNet::SystemAddress *address; // TODO: Encapsulation Leak
		// The packet data
		unsigned char *data;
		// The bit length of the packet data
		unsigned int length;

		~PacketInfo() {
			delete[] data;
		}

		unsigned char getPacketType() { return (unsigned char)this->data[0]; }

	};

};
