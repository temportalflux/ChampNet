#ifndef CHAMPNET_SERVER_SERVER_H
#define CHAMPNET_SERVER_SERVER_H

#include "StateApplication.h"

#include <string>

class GameState;
class PerformanceTracker;

namespace ChampNet
{
	class Packet;
};

class StateServer : public StateApplication
{

	struct PlayerRequest
	{
		unsigned int localID;
		std::string name;
		float colorR, colorG, colorB;
	};

private:
	typedef std::string* PlayerAddress;

	int mClientCount;
	int mPlayerCount;
	PlayerAddress *mpClientAddresses = NULL;
	int *mpPlayerIdToClientId = NULL;
	int **mpClientIdToPlayers = NULL;

	GameState *mpGameState;

	PerformanceTracker *mpTimers;
	double mMsPerUpdate;

public:

	StateServer();
	virtual ~StateServer();

	virtual void onEnterFrom(StateApplication *previous) override;

	/** Author: Dustin Yost
	* Called when a key is marked as down this update
	*/
	virtual void onKeyDown(int i) override;

	/** Author: Dustin Yost
	* Called when some input has been entered (ENTER has been pressed)
	*/
	virtual void onInput(std::string &input);

	/** Author: Dustin Yost
	 * Updates the network
	 */
	virtual void updateNetwork() override;

	/** Author: Dustin Yost
	 * Handles the usage of all the different packet identifiers
	 */
	void handlePacket(ChampNet::Packet *packet);

	virtual void updateGame() override;

	virtual void render() override;

	/** Author: Dustin Yost
	 * Starts the server at whatever address and port are in the state data
	 */
	void start();
	/** Author: Dustin Yost
	 * Disconnects server and notifies clients of disconnection
	 */
	void disconnect();

	/** Author: Dustin Yost
	* Send packet data to the network
	*/
	void sendPacket(const char *address, char *data, int dataSize, bool broadcast);
	/** Author: Dustin Yost
	* Cast some general packet type to data
	*/
	template <typename T>
	void sendPacket(const char *address, T *packet, bool broadcast)
	{
		this->sendPacket(address, (char*)(packet), sizeof(*packet), broadcast);
	}

	/** Author: Dustin Yost
	 * Sends clients the notification of server disconnection
	 */
	void sendDisconnectPacket(const char *address, bool broadcast);

	void deserializePlayerRequests(ChampNet::Packet *pPacket, PlayerRequest *&requests, int &playerRequestCount);

	/** Author: Dustin Yost
	* Finds the next available address slot, returning -1 if none is found
	*/
	bool findNextClientID(unsigned int &id);

	/** Author: Dustin Yost
	* Finds the next available player slot, returning -1 if none is found
	*/
	bool findNextPlayerID(unsigned int &id);

	bool addClient(const char* address, unsigned int &id);
	void removeClient(unsigned int id);
	bool addPlayer(unsigned int clientID, unsigned int localID, unsigned int &playerID,
		std::string name, float colorR, float colorG, float colorB);

	void sendGameState(unsigned char msgID, const char* sender = NULL, bool broadcast = true, int clientID = -1);

};

#endif // CHAMPNET_SERVER_SERVER_H