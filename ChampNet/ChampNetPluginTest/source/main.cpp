
#include <iostream>

#include "Game.h"
#include "StateServer.h"

using namespace std;

int main() {
	cout << "Starting ChampNet server...\n";

	// Create a server
	Game *pGame = new Game(new StateServer());

	// Run the loop for the game
	while (pGame->isRunning())
	{
		pGame->update();
	}

	// Delete the server
	delete pGame;
	pGame = NULL;

	system("pause");
	return 0;
}
