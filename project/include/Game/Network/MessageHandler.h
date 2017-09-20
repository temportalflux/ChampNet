#pragma once

#include "Network\Packets\PacketInfo.h"

class MessageHandler abstract {

public:

	virtual void handlePacket(Network::PacketInfo info) = 0;

};
