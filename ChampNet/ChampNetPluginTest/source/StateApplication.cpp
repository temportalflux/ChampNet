#include "StateApplication.h"

#include "StateData.h"
#include "Win.h"


StateApplication::StateApplication()
{
	mIsRunning = true;
	this->mpState = new StateData();
	mpState->mConsole.consoleWindow = GetForegroundWindow();
}

StateApplication::~StateApplication()
{
	if (mpState != NULL)
	{
		delete mpState;
		mpState = NULL;
	}
}

StateData* StateApplication::getData() const
{
	return this->mpState;
}

void StateApplication::onEnterFrom(StateApplication *previous)
{
	if (previous != NULL)
	{
		if (mpState != NULL)
		{
			delete mpState;
			mpState = NULL;
		}

		this->mpState = previous->mpState;
		previous->mpState = NULL;
	}
	
}

void StateApplication::onExitTo(StateApplication *next)
{

}

/** Author: Dustin Yost
* Collects all input changes
*/
void StateApplication::updateInput()
{
	if (GetForegroundWindow() != this->mpState->mConsole.consoleWindow)
	{
		return;
	}

	byte* mainState = NULL;
	GetKeyboardState(mainState);
	// Gather all keyboard states
	for (int key = 0; key < StateInput::SIZE_KEYBOARD; key++)
	{
		this->mpState->mInput.previous[key] = this->mpState->mInput.keyboard[key];
		this->mpState->mInput.keyboard[key] = (GetAsyncKeyState(key) & 0x8000) != 0;
	}

}

/** Updates the network | STUB */
void StateApplication::updateNetwork() {}

/** Author: Dustin Yost
* Handles all game updates and responses to input/network updates
*/
void StateApplication::updateGame()
{
	this->updateGameForInput(this->mpState->mInput.keyboard, this->mpState->mInput.previous);
}

/** Renders the game to the display | STUB */
void StateApplication::render() {}

/** Author: Dustin Yost
* Updates states according to input updates
*/
void StateApplication::updateGameForInput(const char *current, const char *previous)
{
	// Handle updates from the keyboard
	for (int i = 0; i < StateInput::SIZE_KEYBOARD; i++)
	{
		// Check if the key was pressed this update
		if (!(current[i] && !previous[i]))
		{
			// not pressed this update, so skip
			continue;
		}

		// Notify that a key has been pressed
		this->onKeyDown(i);

	}
}

/** Author: Dustin Yost
* Called when a key is marked as down this update
*/
void StateApplication::onKeyDown(int i)
{
	if (i == VK_ESCAPE)
	{
		this->mIsRunning = false;
	}
}
