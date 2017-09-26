#pragma once

#pragma once

#include "Game\State\StateApplication.h"

// Author: Dustin Yost
class StateGame abstract : public StateApplication {

private:

	virtual void queueNextGameState() override;

	virtual void onEnterFrom(StateApplication *previous);
	void updateGame();
	void render();

public:

	static StateApplication* create(bool isLocalGame);

};
