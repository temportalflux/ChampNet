/*
Dustin Yost 0984932
Jon Trusheim 0979380
EGP-405-02
Project 1
09/21/2017
We certify that this work is entirely our own.
The assessor of this project may reproduce this project and provide
copies to other academic staff, and/or communicate a copy of this project
to a plagiarism-checking service, which may retain a copy of
the project on its database.
*/

#pragma once

#include <vector>
#include <sstream>
#include <RakNet\RakNetTypes.h>
#include <RakNet\WindowsIncludes.h>
#include <Windows.h>
#include "Network\FrameworkDefines.h"
#include <map>

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
	
	// The current line being editted by the user
	std::string currentLine;

	void copyFrom(StateInput *other);

};

// Author: Dustin Yost
struct StateConsole {

	// A reference to the console window
	HWND consoleWindow;
	// The size of the console window
	RECT size;

	void copyFrom(StateConsole *other);

	// Sets the local size variable
	void setSize(RECT rect);
	// Sets the flag on the console window of if the cursor should be active
	void showConsoleCursor(bool showFlag);
	// Sets the position of the cursor on the window
	void setCursorPosition(short x, short y);
	// Generates a string of spaces in length equal to count
	std::string spaceCount(int count);

};

// Author: Dustin Yost
struct StateNetwork {

	enum NetworkType {
		LOCAL, HOST, PEER
	} networkType;

	FrameworkData networkInfo;

	StateNetwork();
	~StateNetwork();

	void copyFrom(StateNetwork *other);

};

struct StateDisplay {

	// A record of all text inputs
	std::vector<std::string> textRecord;

	void copyFrom(StateDisplay *other);

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
		// The state of network
		StateDisplay *display;

		void copyFrom(Data &data);

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
	bool mEscape = false;

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

	bool updateForInput(std::string &latestLine, bool allowEmptyLines);
	void renderConsole();

	/* Author: Dustin Yost
	Pushes a text message to the record of messages to display
	*/
	void pushMessage(const std::string &msg);

	virtual void queueNextGameState() = 0;

};
