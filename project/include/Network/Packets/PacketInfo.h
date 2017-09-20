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

		unsigned int getPacketType() { return this->data[0]; }

	};

};
