#pragma once

struct StateInput {

	static const unsigned int SIZE_KEYBOARD = 256;
	bool previous[StateInput::SIZE_KEYBOARD];
	bool keyboard[StateInput::SIZE_KEYBOARD];

};

class StateApplication {

public:

	struct Data {

		StateInput input[1];

	};

protected:

	StateApplication *mNext, *mPrevious;
	Data mData;

public:
	StateApplication();
	virtual ~StateApplication();

	bool mRunning = true;

	bool hasNext();
	StateApplication* getNext();

	// When exiting this state (we are the previous state)
	// Prepare data
	virtual void onExit();

	// When entering this state (we are the next state)
	// Recieve data
	virtual void onEnterFrom(StateApplication *previous);

	virtual void updateInput();
	virtual void updateNetwork() = 0;
	virtual void updateGame() = 0;
	virtual void render() = 0;

};
