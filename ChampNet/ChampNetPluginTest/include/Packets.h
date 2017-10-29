#pragma once

#pragma pack(push, 1)

struct PacketGeneral
{
	unsigned char id;
};

struct PacketUserID
{
	unsigned char id;
	unsigned int playerId;
};

struct PacketPlayerPosition
{
	unsigned char id;
	unsigned int playerId;
	float posX, posY;
};

#pragma pack(pop)
