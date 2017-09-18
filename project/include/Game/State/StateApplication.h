#pragma once

class StateApplication abstract {

protected:

	StateApplication *mNext, *mPrevious;

	struct Data {

	} data;

	// When exiting this state (we are the previous state)
	// Prepare data
	virtual void onExit(Data *next) = 0;

	// When entering this state (we are the next state)
	// Recieve data
	virtual void onEnter(Data *previous) = 0;

public:
	bool mRunning;

	virtual void updateInput() = 0;
	virtual void updateNetwork() = 0;
	virtual void updateGame() = 0;
	virtual void render() = 0;

};
