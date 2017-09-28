#pragma once

#include "Messaging\GameMessage.h"

class MessageLoadData :public GameMessage
{

public:
	MessageLoadData() :GameMessage(MessageType::DATA_LOAD) {}
	void process();

};