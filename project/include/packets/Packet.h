/*
	Dustin Yost 0984932
	EGP-405-02
	Lab1
	09/03/2017
	We certify that this work is entirely our own.
	The assessor of this project may reproduce this project and provide
	copies to other academic staff, and/or communicate a copy of this project
	to a plagiarism-checking service, which may retain a copy of
	the project on its database.
*/
#pragma once

#include "network\Network.h"

namespace NetworkImpl {
	
	// A list of user implemented methods outside of RakNet
	enum GameMessages
	{
		ID_PLACEHOLDER = Network::BASE_PACKET_ENUM,
		// CLIENT->SERVER, notify server that a client has successfully joined
		ID_CLIENT_JOINED,
		// SERVER->CLIENT, confirm that the server recognizes the client has joined
		ID_CLIENT_JOINED_CONFIRM,
		// CLIENT->SERVER, notify server that a client has left
		ID_CLIENT_LEFT

	};

#pragma pack(push, 1)

	// A simple packet carrying a message
	struct PacketMessage {
		unsigned char typeId;
		char *message;
	};


#pragma pack(pop)
	
};
