#include "Game\InputSystem.h"

#include <string>
#include "Network\Framework.h"

namespace Game {

	InputSystem::InputSystem() {
		this->mpConsoleWindow = GetForegroundWindow();
	}

	void InputSystem::update(bool &shouldExit) {

		// Do not handle WINAPI input if this window is not active
		if (GetForegroundWindow() != this->mpConsoleWindow) {
			return;
		}

		// check for escape key
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			shouldExit = true;
		}

	}

}
