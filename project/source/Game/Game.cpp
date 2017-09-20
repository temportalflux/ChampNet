/*
Dustin Yost 0984932
Jon Trusheim 0979380
EGP-405-02
Project 1
09/21/2017
We certify that this work is entirely our own.
The assessor of this project may reproduce this project and provide
copies to other academic staff, and/or communicate a copy of this project
to a plagiarism-checking service, which may retain a copy of
the project on its database.
*/
#include "Game\Game.h"

#include "Game\State\StateApplication.h"
#include "Game\State\StateLobby.h"
	
Game::Game() {
}

Game::~Game() {
	delete mpStateGame;
}

void Game::run() {
	this->startup();
	this->runLoop();
}

/* Author: Dustin Yost
Handles starting up the game object, and setting console window properties
*/
void Game::startup() {
	
	// Create the first game state
	this->mpStateGame = new StateLobby();
	this->mpStateGame->onEnterFrom(NULL);

	// Get the console window and set in the game state
	HWND consoleWindow = GetConsoleWindow();
	this->mpStateGame->setWindow(consoleWindow);

	// Restrict console window from being resized
	SetWindowLongPtr(consoleWindow, GWL_STYLE, GetWindowLongPtr(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	// yOffset, xOffset, height, width
	RECT rect = { 200, 200, 600, 800 };
	// Set the size of the console window
	this->mpStateGame->console()->setSize(rect);
	MoveWindow(consoleWindow, rect.top, rect.left, rect.bottom, rect.right, TRUE);

	// Prevent the cursor from blinking
	this->mpStateGame->console()->showConsoleCursor(false);

}

StateApplication* Game::getGameState() {
	return this->mpStateGame;
}

/* Author: Dustin Yost
Handles updating the game state and cycling to the next state if there is one
*/
void Game::runLoop() {
	// Search for packet messages
	while (this->mpStateGame->mRunning)
	{
		this->mpStateGame->updateInput();
		this->mpStateGame->updateNetwork();
		this->mpStateGame->updateGame();
		this->mpStateGame->render();

		// Handle state transitions
		if (this->mpStateGame->hasNext()) {
			// Cache the previous state
			StateApplication *tmp = this->mpStateGame;
			// Notify previous state of exit
			this->mpStateGame->onExit();
			// Get the next state
			this->mpStateGame = this->mpStateGame->getNext();
			// Notify next state of entrance from previous state
			this->mpStateGame->onEnterFrom(tmp);
		}
	}
}
