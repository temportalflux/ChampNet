/*
	Dustin Yost 0984932
	EGP-405-02
	Project 1
	09/21/2017
	We certify that this work is entirely our own.
	The assessor of this project may reproduce this project and provide
	copies to other academic staff, and/or communicate a copy of this project
	to a plagiarism-checking service, which may retain a copy of
	the project on its database.
*/
#include "Game\InputSystem.h"

#include <string>
#include "Network\Framework.h"

// Author: Dustin Yost
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
