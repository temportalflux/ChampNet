#include "Messaging\MessageMusic.h"

#include "Util\Game.h"
#include "Graphics\Audio.h"

MessageMusic::MessageMusic(State state)
	:GameMessage(MUSIC), mState(state)
{
}

MessageMusic::~MessageMusic()
{
}

void MessageMusic::process()
{
	switch (mState) {
		case PAUSE:
			gpGame->getMusic()->pause();
			break;
		case RESTART:
			gpGame->getMusic()->restart();
			break;
		default:
			break;
	}
}
