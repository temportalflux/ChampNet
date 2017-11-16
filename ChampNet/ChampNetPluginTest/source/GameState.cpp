#include "GameState.h"

GameState::GameState(int id)
{
	this->clientID = id;
	this->players = std::map<unsigned int, GameState::Player>();
}

GameState::~GameState()
{

}

void GameState::addPlayer(unsigned int clientID, unsigned int playerID,
	std::string name, float colorR, float colorG, float colorB)
{
	Player player;
	player.clientID = clientID;
	player.playerID = playerID;
	player.name = name;
	player.colorR = colorR;
	player.colorG = colorG;
	player.colorB = colorB;
	player.posX = player.posY = player.posZ = 0;
	player.velX = player.velY = player.velZ = 0;
	player.accX = player.accY = player.accZ = 0;
	player.inBattle = false;
	this->players[playerID] = player;
}

char* GameState::serializeForClient(unsigned char packetID, unsigned int clientID, int &dataLength)
{
	const int size = sizeof(packetID)
		// clientID
		+ sizeof(unsigned int)
		// number of players
		+ sizeof(int)
		// amount of space required for players
		+ (this->players.size() * Player::SIZE)
		;
	dataLength = size;
	char *data = new char[size];
	char *pos = data;

	// write the packetID
	*((char *)pos) = packetID; pos += sizeof(packetID);
	// write the client ID
	*((int *)pos) = clientID; pos += sizeof(unsigned int);
	// write player count
	*((int *)pos) = this->players.size(); pos += sizeof(int);

	// write players
	for (auto const &entry : this->players)
	{
		Player player = entry.second;

		// write clientID
		*((int *)pos) = player.clientID; pos += sizeof(unsigned int);
		// write playerID
		*((int *)pos) = player.playerID; pos += sizeof(unsigned int);

		// write name
		// NOTE: this does not use ALL of the name byte, if the name is < SIZE_MAX_NAME characters
		int nameLength = player.name.length();
		nameLength = nameLength < Player::SIZE_MAX_NAME ? nameLength : Player::SIZE_MAX_NAME;
		*((int *)pos) = nameLength; pos += sizeof(int);
		for (int i = 0; i < nameLength; i++)
		{
			*((int *)pos) = player.name[i]; pos += sizeof(char);
		}

		// write color
		*((float *)pos) = player.colorR; pos += sizeof(float);
		*((float *)pos) = player.colorG; pos += sizeof(float);
		*((float *)pos) = player.colorB; pos += sizeof(float);
		// write position
		*((float *)pos) = player.posX; pos += sizeof(float);
		*((float *)pos) = player.posY; pos += sizeof(float);
		*((float *)pos) = player.posZ; pos += sizeof(float);
		// write velocity
		*((float *)pos) = player.velX; pos += sizeof(float);
		*((float *)pos) = player.velY; pos += sizeof(float);
		*((float *)pos) = player.velZ; pos += sizeof(float);
		// write acceleration
		*((float *)pos) = player.accX; pos += sizeof(float);
		*((float *)pos) = player.accY; pos += sizeof(float);
		*((float *)pos) = player.accZ; pos += sizeof(float);
		// write inBattle
		*((int *)pos) = player.inBattle; pos += sizeof(bool);
		
	}

	// data is now filled with the gamestate data
	return data;
}
