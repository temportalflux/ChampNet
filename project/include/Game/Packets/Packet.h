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
#include <string>

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
	ID_USER_LISTING,

};

static const unsigned int PACKET_MAX_SIZE_CONTENT = 31;
static const unsigned int PACKET_MAX_SIZE_TEXT = 63;
static const unsigned int PACKET_SIZE_IPV4 = 15;
static const unsigned int PACKET_USERNAME_SIZE_CONTENT = 48;

#pragma pack(push, 1)

struct PacketNotification {
	unsigned char packetID;
};

// Author: Dustin Yost
struct PacketString {
	unsigned char packetID;
	char content[PACKET_MAX_SIZE_CONTENT];
};

// GameMessages::ID_USERNAME
struct PacketUsername { // total: 64
	unsigned char packetID; // 1
	char address[PACKET_SIZE_IPV4]; // 15
	char content[PACKET_USERNAME_SIZE_CONTENT]; // 48
};

// Author: Dustin Yost
// GameMessages::ID_NEW_CLIENT_JOINED
struct PacketStringLarge {
	unsigned char packetID;
	int clientID;
	char content[PACKET_MAX_SIZE_TEXT];
};

// Author: Dustin Yost
// GameMessages::ID_CLIENT_NUMBER
struct PacketUInt {
	unsigned char packetID;
	int clientID;
};

// Author: Dustin Yost
// GameMessages::ID_CHAT_MESSAGE
struct PacketChatMessage {
	unsigned char packetID;
	int clientID;
	char username[PACKET_MAX_SIZE_CONTENT];
	char message[PACKET_MAX_SIZE_TEXT + 1];
};

// Author: Dustin Yost
// GameMessages::ID_CHAT_MESSAGE
struct PacketStringLargeUsername {
	unsigned char packetID;
	char username[PACKET_MAX_SIZE_CONTENT];
	char message[PACKET_MAX_SIZE_TEXT + 1];
};

#pragma pack(pop)

void writeToCharData(char *data, const std::string &string, size_t maxSizeOfData);
