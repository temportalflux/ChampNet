#pragma once

struct StateInput {

	static const unsigned int SIZE_KEYBOARD = 256;
	unsigned char keyboard[StateInput::SIZE_KEYBOARD];

};

struct StateNetwork {

};

struct StateGame {

	bool running;
	StateInput input[1];
	StateNetwork network[1];
	
	enum EnumState {
		NONE = 0,
		LOGIN,
	} stateCurrent, stateNext;

};
