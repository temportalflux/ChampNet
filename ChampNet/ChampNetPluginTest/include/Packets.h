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

struct PacketClientPlayerID
{
	unsigned char id;
	unsigned int clientID;
	unsigned int playerID;
};

struct PacketPlayerScoreboardInfo // created to just increment score/set new rank
{
	unsigned char id;
	unsigned int clientID;
	unsigned int playerID;
	unsigned int score;
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

struct PacketBattlePromptSelection
{
	unsigned char id;

	unsigned int playerAId;
	// -1 if invalid
	int playerASelection;
	// -1 if invalid
	int playerAChoice;

	unsigned int playerBId;
	// -1 if invalid
	int playerBSelection;
	// -1 if invalid
	int playerBChoice;

};

struct PacketBattleSelection
{
	unsigned char id;
	unsigned int playerId;
	unsigned int playerIdOpponent;
	unsigned int selection;
	unsigned int choice;
};

#pragma pack(pop)

enum BattleSelection : unsigned int
{
	ATTACK, SWAP, FLEE,
};
