#pragma once

#include "Messaging\GameMessage.h"

class MessageMusic : public GameMessage
{

public:
	enum State {
		PAUSE, RESTART
	};

private:
	State mState;

public:

	MessageMusic(State state);
	~MessageMusic();

	void process();

};
