#include "Messaging\MessageGameExit.h"

#include "Util\Defines.h"
#include "Util\Game.h"

MessageGameExit::MessageGameExit() : GameMessage(MessageType::GAME_EXIT) {
}

MessageGameExit::~MessageGameExit()
{
}

void MessageGameExit::process()
{
	gpGame->exit();
}
