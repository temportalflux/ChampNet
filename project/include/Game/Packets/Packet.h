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

#include "Network\NetworkDefines.h"

// Author: Dustin Yost
// A list of user implemented methods outside of RakNet
enum GameMessages
{
	ID_PLACEHOLDER = Network::BASE_PACKET_ENUM,
	// CLIENT->SERVER, notify server that a client has successfully joined
	//ID_CLIENT_JOINED,
	// SERVER->CLIENT, confirm that the server recognizes the client has joined
	//ID_CLIENT_JOINED_CONFIRM,
	// CLIENT->SERVER, notify server that a client has left
	ID_CLIENT_LEFT,

	// Handshake exchange
	// Client->Server; client responds to connection by sendings its username
	ID_USERNAME,
	// Server->Client; broadcast welcome message to all clients (a new client has joined)
	ID_NEW_CLIENT_JOINED,
	// Server->Client; server tells client its ID number
	ID_CLIENT_NUMBER,

	// Messaging
	// Client->Client; Client broadcast to some username (or all)
	ID_CHAT_MESSAGE,

	//sends a message to a single other client 
	ID_PRIVATE_MESSAGE,
	// Misc
	ID_SEND_ALL, // used to notify incoming users of a list of current usernames active

};

#pragma pack(push, 1)

struct PacketNotification {
	unsigned char packetID;
};

// Author: Dustin Yost
// GameMessages::ID_USERNAME
struct PacketString : Packet {
	static const unsigned int MAX_SIZE_CONTENT = 31;
	unsigned char packetID;
	char content[PacketString::MAX_SIZE_CONTENT];
};

// Author: Dustin Yost
// GameMessages::ID_NEW_CLIENT_JOINED
struct PacketStringLarge : Packet {
	unsigned char packetID;
	char content[511];
};

// Author: Dustin Yost
// GameMessages::ID_CLIENT_NUMBER
struct PacketUInt : Packet {
	unsigned char packetID;
	unsigned int clientID;
};

// Author: Dustin Yost
// GameMessages::ID_CHAT_MESSAGE
struct PacketChatMessage : Packet {
	unsigned char packetID;
	char username[31];
	char message[64];
};


#pragma pack(pop)
