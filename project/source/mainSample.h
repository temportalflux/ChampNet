// INTRO TO NETWORKED ARCHITECTURE - ASYNC CHAT ROOM APPLICATION'

#include <Windows.h>

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

void updateInput(StateInput* state) {
	// Get mouse, keyboard, controller, etc state
	// store somewhere for later

	// example
	GetKeyboardState(state->keyboard);

}

void updateNetwork(StateNetwork* state) {
	// recieve all packets
	// for each packet:
	//	process message
	//	track data for later

	// in state:
	//  store peer interface
	//  process loop
}

void updateState(StateGame* state) {
	// account for all previous updates (input & network)
	// change application wrt time and other updates

	// e.g. chat room
	//	update char stream
}

void render(const StateGame* state) {
	// clear the buffer (for text rendering): system("cls")
	// display state to display medium

	// e.g. chat room
	//	display chat stream (all of it)
}

int main() {
	
	StateGame stateGame[1];
	stateGame->running = true;

	while (stateGame->running) {
		updateInput(stateGame->input);
		updateNetwork(stateGame->network);
		updateState(stateGame);
		render(stateGame);
	}

	return 0;
}
