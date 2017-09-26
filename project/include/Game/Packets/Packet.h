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

	// Host->Peer: ID_CONNECTION_REQUEST_ACCEPTED - server has accepted the client
	// Peer->Host: ID_USER_CONNECTED - client as recieved the connection, and is ready to start
	ID_USER_CONNECTED,
	// Host->Peer: ID_START_GAME - server has acknowledged client, and the game is starting
	ID_START_GAME,

	// ID Messages for valid move submitions
	ID_MOVE_SUBMIT_0, ID_MOVE_SUBMIT_1, ID_MOVE_SUBMIT_2,
	ID_MOVE_SUBMIT_3, ID_MOVE_SUBMIT_4, ID_MOVE_SUBMIT_5,
	ID_MOVE_SUBMIT_6, ID_MOVE_SUBMIT_7, ID_MOVE_SUBMIT_8,

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
