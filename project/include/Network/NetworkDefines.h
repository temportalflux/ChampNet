#pragma once

#include <RakNet\MessageIdentifiers.h>  // MessageID

struct Packet {};

namespace Network {
	const int BASE_PACKET_ENUM = ID_USER_PACKET_ENUM + 1;
};
