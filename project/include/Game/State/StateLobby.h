#pragma once

#include "Game\State\StateApplication.h"

class StateLobby : public StateApplication {

private:

	void onEnter(Data *previous);
	void onExit(Data *next);

	void updateInput();
	void updateNetwork();
	void updateGame();
	void render();

};
