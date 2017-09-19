#pragma once

#include "Framework.h"
#include <string>

class Client : public Framework {

protected:
	std::string mAddressServerStr;

public:
	Client(Network::PacketHandler* packetHandler, FrameworkData data);
	virtual ~Client();

protected:

	virtual Network::Network* createNetwork();

};
