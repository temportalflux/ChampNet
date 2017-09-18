#pragma once

#include "Game\State\StateApplication.h"

class StateGameLocal : public StateApplication {

private:

	void updateNetwork();
	void updateGame();
	void render();

};
