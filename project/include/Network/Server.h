#pragma once

#include "Framework.h"

class Server : public Framework {

protected:
	unsigned int mMaxClients;

	virtual void prompt();
	virtual Network::Network* createNetwork();

public:
	Server(Network::PacketHandler* packetHandler);
	virtual ~Server();

};
