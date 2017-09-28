#pragma once

#include "Messaging\GameMessage.h"

class Vector2D;

class MessageSpawn : public GameMessage
{

public:
	MessageSpawn();
	~MessageSpawn();

	void process();

};
