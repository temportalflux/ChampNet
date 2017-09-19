#pragma once

#include <vector>
#include <sstream>
#include <Windows.h>
#include "Network\FrameworkDefines.h"

// Author: Dustin Yost
struct StateInput {

	// A static reference to the max quantity of keys
	static const unsigned int SIZE_KEYBOARD = 256;
	// The previous state of the keyboard (the iteration prior to the current state)
	bool previous[StateInput::SIZE_KEYBOARD];
	// The current state of the keyboard
	bool keyboard[StateInput::SIZE_KEYBOARD];
	// If the capslock is enabled
	bool isCaps;
	
	// A record of all text inputs
	// TODO: Move to game state
	std::vector<std::string> textRecord;
	// The current line being editted by the user
	std::string currentLine;

};

// Author: Dustin Yost
struct StateConsole {

	// A reference to the console window
	HWND consoleWindow;
	// The size of the console window
	RECT size;

	// Sets the local size variable
	void setSize(RECT rect);
	// Sets the flag on the console window of if the cursor should be active
	void showConsoleCursor(bool showFlag);
	// Sets the position of the cursor on the window
	void setCursorPosition(short x, short y);
	// Generates a string of spaces in length equal to count
	std::string spaceCount(int count);

};

struct StateNetwork {

	// If this is a server (false for clients)
	bool isServer;
	FrameworkData networkInfo;

};

// Author: Dustin Yost
class StateApplication {

public:

	struct Data {

		Data();
		~Data();

		// The state of the console window
		StateConsole *console;
		// The state of input
		StateInput *input;
		// The state of network
		StateNetwork *network;

	};

protected:

	// The previous and next state
	// If next is non-null, then it will be transitioned to in the next update loop (see Game::update)
	StateApplication *mNext, *mPrevious;
	// The state data
	Data mData;

public:
	// If the update loop should continue
	bool mRunning = true;

	StateApplication();
	virtual ~StateApplication();

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

	bool hasNext();
	StateApplication* getNext();

	// Returns the console state
	StateConsole* console();
	// Sets the console window reference
	void setWindow(HWND window);
	
};
