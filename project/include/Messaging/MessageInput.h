#pragma once

#include "Messaging\GameMessage.h"

class InputSystem;

class MessageInput :public GameMessage
{

private:
	InputSystem* mInput;
	bool mIsMouse;
	int mId;

public:
	MessageInput(InputSystem *input, bool isMouse, int id);
	~MessageInput();

	void process();

};
