#pragma once

#include "Messaging\GameMessage.h"

class MessageGameExit :public GameMessage
{

public:
	MessageGameExit();
	~MessageGameExit();

	void process();

};