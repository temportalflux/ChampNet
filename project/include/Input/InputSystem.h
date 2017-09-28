#pragma once

#include <Trackable.h>

// Handles all input from SDL
class InputSystem : public Trackable {

private:
	// The last recorded mouse position x
	int mMouseX;
	// The last recorded mouse position y
	int mMouseY;

public:
	InputSystem();
	
	// Updates input and fires MessageInput as necessary
	void process();

	// Returns the last recorded mouse x-coord
	int getMouseX();
	// Returns the last recorded mouse y-coord
	int getMouseY();

};
