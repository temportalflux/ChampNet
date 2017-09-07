#pragma once

#include "Framework.h"

class Client : public Framework {

protected:
	std::string mAddressServerStr;

public:
	Client(Network::PacketHandler* packetHandler);
	virtual ~Client();

protected:

	virtual void prompt();
	virtual Network::Network* createNetwork();

};
