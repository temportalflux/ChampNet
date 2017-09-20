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
	display = new StateDisplay();
}

// Author: Dustin Yost
StateApplication::Data::~Data() {
	delete console;
	delete input;
	delete network;
	delete display;
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

/* Author: Dustin Yost
Handles updates from the input state and handling how those strings are cached
*/
bool StateApplication::updateForInput(std::string &latestLine, bool allowEmptyLines) {
	// Cache keyboard state
	bool *current = this->mData.input->keyboard;
	bool *previous = this->mData.input->previous;

	bool enteredNewLine = false;

	// Handle updates from the keyboard
	for (int i = 0; i < StateInput::SIZE_KEYBOARD; i++) {
		// Check if the key was pressed this update
		if (current[i] && !previous[i]) {
			// Check all keys that were pressed
			switch (i) {
				case VK_ESCAPE: // special effect, set the application to stop running
					this->mRunning = false;
					break;
				case VK_RETURN: // special effect, push the text line into the records buffer
								// only if there is text in the buffer
					if (allowEmptyLines || this->mData.input->currentLine.length() > 0) {

						// Set return to mark that a new line was recorded
						enteredNewLine = true;

						// Mark the latest line for the caller
						latestLine = this->mData.input->currentLine;

						// push back the string
						this->mData.display->textRecord.push_back(latestLine);

						// empty the string
						this->mData.input->currentLine = "";
					}
					break;
				case VK_BACK: // special effect, remove the last character in the stream
					{
						// get teh current line
						std::string str = this->mData.input->currentLine;
						// ensure that the line has content to begin with
						if (str.length() > 0) {
							// find the substring without the last character, the current string buffer becomes that
							this->mData.input->currentLine = str.substr(0, str.length() - 1);
						}
					}
					break;
				case VK_CAPITAL: // special effect, toggle the capslock
					this->mData.input->isCaps = !this->mData.input->isCaps;
					break;
					// These should not have an input effect
				case VK_SHIFT: break;
				case VK_LSHIFT: break;
				case VK_RSHIFT: break;
				case VK_CONTROL: break;
					// Handle all remaining cases (letters and specials)
				default:
					// get the character from the virtual key index
					char character = (char)MapVirtualKey(i, MAPVK_VK_TO_CHAR);
					// handle upper/lower case letters: check to see if the shift key is down
					if (!(current[VK_SHIFT] || current[VK_LSHIFT] || current[VK_RSHIFT] || this->mData.input->isCaps)) {
						character = tolower(character);
					}
					// push the character onto the text buffer
					this->mData.input->currentLine.push_back(character);
					break;
			}
		}
	}

	return enteredNewLine;
}
