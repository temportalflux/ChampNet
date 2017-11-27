#ifndef CHAMPNET_SERVER_GAMESTATE_H
#define CHAMPNET_SERVER_GAMESTATE_H

#include <map>

class GameState
{

public:
	
	struct Player
	{
		const static int SIZE_MAX_NAME = 10;
		const static int SIZE = 0
			// clientID
			+ sizeof(unsigned int)
			// playerID
			+ sizeof(unsigned int)
			// localID
			+ sizeof(unsigned int)
			// name
			+ sizeof(int) + (SIZE_MAX_NAME * (sizeof(char) * 2))
			// color
			+ (sizeof(float) * 3)
			// position
			+ (sizeof(float) * 3)
			// velocity
			+ (sizeof(float) * 3)
			// acceleration
			+ (sizeof(float) * 3)
			// inBattle
			+ sizeof(bool)
			// Wins
			+ sizeof(unsigned int)
			// Rank
			+ sizeof(unsigned int)
			;

		// the clientID of the controlling client
		unsigned int clientID;
		// the playerID of the character
		unsigned int playerID;
		// the localID of the character
		unsigned int localID;
		// the name of the character
		std::string name;
		// the highlight color of the character
		float colorR, colorG, colorB;
		// the position
		float posX, posY, posZ;
		// velocity
		float velX, velY, velZ;
		// and acceleration
		float accX, accY, accZ;
		// if the player is in battle
		bool inBattle;
		// Number of wins the player has
		unsigned int wins;
		// Rank of player on the scoreboard
		unsigned int rank;
	};

	// The ID of the client this gamestate is in
	int clientID;
	std::map<unsigned int, Player> players;

	GameState(int id);
	~GameState();

	void addPlayer(unsigned int clientID, unsigned int playerID, unsigned int localID,
		std::string name, float colorR, float colorG, float colorB);
	void removePlayer(unsigned int playerID);

	char* serializeForClient(unsigned char packetID, int clientID, int &dataLength);

};

#endif