#pragma once

#include "Messaging\GameMessage.h"

class Vector2D;

class MessageDelete : public GameMessage
{

public:
	MessageDelete();
	~MessageDelete();

	void process();

};
