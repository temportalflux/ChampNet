#pragma once

#include "Framework.h"

class Client : public Framework {

protected:
	std::string mAddressServerStr;

public:
	Client(Network::PacketHandler* packetHandler, FrameworkData data);
	virtual ~Client();

protected:

	virtual Network::Network* createNetwork();

};
