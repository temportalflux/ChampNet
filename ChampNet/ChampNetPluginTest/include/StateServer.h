#ifndef CHAMPNET_SERVER_SERVER_H
#define CHAMPNET_SERVER_SERVER_H

#include "StateApplication.h"

#include <string>

namespace ChampNet
{
	class Packet;
};

class StateServer : public StateApplication
{

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

	void handlePacket(ChampNet::Packet *packet);

	void sendPacket(const char *address, char *data, int dataSize, bool broadcast);
	template <typename T>
	void sendPacket(const char *address, T *packet, bool broadcast)
	{
		this->sendPacket(address, (char*)(packet), sizeof(*packet), broadcast);
	}

	virtual void render() override;

	void start();
	void disconnect();

};

#endif // CHAMPNET_SERVER_SERVER_H