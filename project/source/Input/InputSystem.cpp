#include "Input\InputSystem.h"

#include "Util\Defines.h"
#include "Util\Game.h"
#include "Messaging\GameMessageManager.h"
#include "Messaging\MessageInput.h"

#include <SDL.h>

InputSystem::InputSystem() {

}

void InputSystem::process() {	
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_MOUSEMOTION:
				// Update the mouse coords
				this->mMouseX = e.motion.x;
				this->mMouseY = e.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				// Send button click events
				MESSAGE_MANAGER->addMessage(new MessageInput(this, true, (int)e.button.button), 0);
				break;
			case SDL_KEYDOWN:
				// Send keyboard down events
				MESSAGE_MANAGER->addMessage(new MessageInput(this, false, e.key.keysym.scancode), 0);
				break;
			default:
				break;
		}
	}

}

int InputSystem::getMouseX() {
	return mMouseX;
}

int InputSystem::getMouseY() {
	return mMouseY;
}
