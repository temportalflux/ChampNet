#pragma once

struct StateInput {

	unsigned char keyboard[256];

};

struct StateNetwork {

};


struct StateGame {

	bool running;
	StateInput input[1];
	StateNetwork network[1];

};
