#pragma once

#include "Game\State\State.h"

class StateLogin : public State {

public:
	virtual ~StateLogin();

	virtual void handleInput(StateInput* state) override;
	virtual void handleGameUpdates(StateApplication* state, Game* game) override;

};