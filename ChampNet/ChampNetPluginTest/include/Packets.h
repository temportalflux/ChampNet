#pragma once

// All the possible packet configurations

#pragma pack(push, 1)

struct PacketGeneral
{
	unsigned char id;
};

struct PacketUserID
{
	unsigned char id;
	unsigned int dataID;
};

struct PacketPlayerPosition
{
	unsigned char id;
	unsigned int clientID;
	unsigned int playerID;
	float posX, posY, velX, velY;
};

struct PacketUserIDDouble
{
	unsigned char id;
	unsigned int playerIdSender;
	unsigned int playerIdReceiver;
};

struct PacketUserIDTriple
{
	unsigned char id;
	unsigned int playerIdSender;
	unsigned int playerIdReceiver;
	unsigned int playerIdThird;
};

struct PacketBattleResponse
{
	unsigned char id;
	unsigned int playerIdSender;
	unsigned int playerIdReceiver;
	bool accepted;
};

#pragma pack(pop)
