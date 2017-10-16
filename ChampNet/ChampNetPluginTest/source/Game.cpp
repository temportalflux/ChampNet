#include "Game.h"
#include <cstdlib>

#include "Win.h"

/** Author: Dustin Yost
* A base class for all update loop based game information (runs the game)
*/
Game::Game()
{
	mIsRunning = true;
	mpState = new StateGame();
	mpState->mConsole.consoleWindow = GetForegroundWindow();
}

Game::~Game()
{
	if (mpState != NULL)
	{
		delete mpState;
		mpState = NULL;
	}
}

/** Author: Dustin Yost
* Handles all gameloop updates
*/
void Game::update()
{
	this->updateInput();
	this->updateNetwork();
	this->updateGame();
	this->render();
}

/** Author: Dustin Yost
* Collects all input changes
*/
void Game::updateInput()
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
void Game::updateNetwork() {}

/** Author: Dustin Yost
* Handles all game updates and responses to input/network updates
*/
void Game::updateGame()
{
	this->updateGameForInput(this->mpState->mInput.keyboard, this->mpState->mInput.previous);
}

/** Author: Dustin Yost
* Updates states according to input updates
*/
void Game::updateGameForInput(const char *current, const char *previous)
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
void Game::onKeyDown(int i)
{
	if (i == VK_ESCAPE)
	{
		this->mIsRunning = false;
	}
}

/** Renders the game to the display | STUB */
void Game::render() {}
