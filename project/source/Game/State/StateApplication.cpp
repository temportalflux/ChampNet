#include "Game\State\StateApplication.h"
#include <cstdlib>
#include <Windows.h>

/* Author: Dustin Yost
Set the size variable to reflect the console window
*/
void StateConsole::setSize(RECT rect) {
	this->size = rect;
}

/* Author: Dustin Yost
Sets the flag on the console window of if the cursor should be active
*/
void StateConsole::showConsoleCursor(bool showFlag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

/* Author: Dustin Yost
Sets the position of the cursor on the window
*/
void StateConsole::setCursorPosition(short x, short y) {
	COORD cur = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
}

/* Author: Dustin Yost
Generates a string of spaces in length equal to count
*/
std::string StateConsole::spaceCount(int count) {
	std::stringstream s;
	for (int i = 0; i < count; i++) s << ' ';
	return s.str();
}

// Author: Dustin Yost
StateApplication::Data::Data() {
	console = new StateConsole();
	input = new StateInput();
	network = new StateNetwork();
}

// Author: Dustin Yost
StateApplication::Data::~Data() {
	delete console;
	delete input;
	delete network;
}

// Author: Dustin Yost
StateApplication::StateApplication() {
	mNext = NULL;
	mPrevious = NULL;
}

// Author: Dustin Yost
StateApplication::~StateApplication() {
	if (mPrevious != NULL) {
		delete mPrevious;
		mPrevious = NULL;
	}
	if (mNext != NULL) {
		delete mNext;
		mNext = NULL;
	}
}

void StateApplication::onExit() {}

// Author: Dustin Yost
void StateApplication::onEnterFrom(StateApplication *previous) {
	this->mPrevious = previous;
	// will be null if this is the first state
	if (previous != NULL) {
		this->mData = previous->mData;
	}
}

/* Author: Dustin Yost
Handles caching the keyboard state in the input state data
*/
void StateApplication::updateInput() {
	byte* mainState = NULL;
	GetKeyboardState(mainState);
	// Gather all keyboard states
	for (int key = 0; key < StateInput::SIZE_KEYBOARD; key++) {
		this->mData.input->previous[key] = this->mData.input->keyboard[key];
		this->mData.input->keyboard[key] = (GetAsyncKeyState(key) & 0x8000) != 0;
	}
}

bool StateApplication::hasNext() {
	return mNext != NULL;
}

StateApplication* StateApplication::getNext() {
	return mNext;
}

/* Author: Dustin Yost
Sets the console window reference
*/
void StateApplication::setWindow(HWND window) {
	this->mData.console->consoleWindow = window;
}

/* Author: Dustin Yost
Returns the console state
*/
StateConsole* StateApplication::console() {
	return mData.console;
}
