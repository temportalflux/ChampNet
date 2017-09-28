#include "Messaging\MessageInput.h"

#include <SDL.h>

#include <Vector2D.h>

#include "Input\InputSystem.h"
#include "Util\Defines.h"
#include "Util\Game.h"
#include "Messaging\GameMessageManager.h"
#include "Messaging\PlayerMoveToMessage.h"
#include "Messaging\MessageGameExit.h"
#include "Messaging\MessageSpawn.h"
#include "Messaging\MessageDelete.h"
#include "Messaging\MessageLoadData.h"
#include "Messaging\MessageMusic.h"

MessageInput::MessageInput(InputSystem *input, bool isMouse, int id)
	: GameMessage(MessageType::USER_INPUT)
	, mInput(input)
	, mIsMouse(isMouse)
	, mId(id) {
}

MessageInput::~MessageInput()
{
}

void MessageInput::process()
{
	GameMessage* pMessage = NULL;
	if (this->mIsMouse) {
		// Mouse Button
		switch (mId) {
			case 1:
				/*
				pMessage = new PlayerMoveToMessage(Vector2D(
					this->mInput->getMouseX(),
					this->mInput->getMouseY()
				));
				//*/
				break;
			default:
				break;
		}
	}
	else {
		// Keyboard
		switch (mId) {
			case SDL_SCANCODE_ESCAPE:
				pMessage = new MessageGameExit();
				break;
			case SDL_SCANCODE_A:
				pMessage = new MessageSpawn();
				break;
			case SDL_SCANCODE_D:
				pMessage = new MessageDelete();
				break;
			case SDL_SCANCODE_R:
				pMessage = new MessageLoadData();
				break;
			case SDL_SCANCODE_P:
				pMessage = new MessageMusic(MessageMusic::PAUSE); // pause the music
				break;
			case SDL_SCANCODE_O:
				pMessage = new MessageMusic(MessageMusic::RESTART); // restart the music
				break;
			default:
				break;
		}
	}
	if (pMessage != NULL) {
		MESSAGE_MANAGER->addMessage(pMessage, 0);
	}
}
