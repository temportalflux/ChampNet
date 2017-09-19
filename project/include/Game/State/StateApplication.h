#pragma once

#include <vector>
#include <sstream>
#include <Windows.h>

struct StateInput {

	static const unsigned int SIZE_KEYBOARD = 256;
	bool previous[StateInput::SIZE_KEYBOARD];
	bool keyboard[StateInput::SIZE_KEYBOARD];
	bool isCaps;
	
	std::vector<std::string> textRecord;
	std::string currentLine;

};

struct StateConsole {

	HWND consoleWindow;
	RECT size;

	void setSize(RECT rect);
	void showConsoleCursor(bool showFlag);
	void setCursorPosition(int x, int y);
	std::string spaceCount(int count);

};

class StateApplication {

public:

	struct Data {

		Data();
		~Data();

		StateConsole *console;
		StateInput *input;

	};

protected:

	StateApplication *mNext, *mPrevious;
	Data mData;

public:
	StateApplication();
	virtual ~StateApplication();

	bool mRunning = true;

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

	void setWindow(HWND window);
	bool hasNext();
	StateApplication* getNext();

	StateConsole* console();

};
