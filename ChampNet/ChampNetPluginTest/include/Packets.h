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

#pragma pack(pop)
