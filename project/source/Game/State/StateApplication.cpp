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

#include "Game\State\StateApplication.h"
#include <cstdlib>
#include "lib\LibWindows.h"
#include <iostream>

void StateInput::copyFrom(StateInput *other) {
	for (int i = 0; i < StateInput::SIZE_KEYBOARD; i++) {
		this->previous[i] = other->previous[i];
		this->keyboard[i] = other->keyboard[i];
	}
	this->isCaps = other->isCaps;
	this->currentLine = other->currentLine;
}

void StateInput::resetInput() {
	for (int i = 0; i < StateInput::SIZE_KEYBOARD; i++) {
		this->previous[i] = false;
		this->keyboard[i] = false;
	}
}

StateNetwork::StateNetwork() {
}

StateNetwork::~StateNetwork() {
}

void StateNetwork::copyFrom(StateNetwork *other) {
	this->networkType = other->networkType;
	this->networkInfo = other->networkInfo;
}

void StateConsole::copyFrom(StateConsole *other) {
	this->size = other->size;
	this->consoleWindow = other->consoleWindow;
}

void StateDisplay::copyFrom(StateDisplay *other) {
	this->textRecord = other->textRecord;
}

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
	if (console != NULL) delete console;
	if (input != NULL) delete input;
	if (network != NULL) delete network;
	if (display != NULL) delete display;
}

void StateApplication::Data::copyFrom(Data &other) {
	console->copyFrom(other.console);
	input->copyFrom(other.input);
	network->copyFrom(other.network);
	display->copyFrom(other.display);
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

	// will be null if this is the first state
	if (previous != NULL) {
		this->mData.copyFrom(previous->mData);
		previous->mNext = NULL;
		delete previous;
	}

	std::fill_n(this->mData.input->previous, StateInput::SIZE_KEYBOARD, false);
	std::fill_n(this->mData.input->keyboard, StateInput::SIZE_KEYBOARD, false);
	this->mData.input->currentLine = "";

}

/* Author: Dustin Yost
Handles caching the keyboard state in the input state data
*/
void StateApplication::updateInput() {
	if (GetForegroundWindow() != this->mData.console->consoleWindow) {
		return;
	}

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
					//this->mRunning = false;
					this->mEscape = true;
					break;
				case VK_RETURN: // special effect, push the text line into the records buffer
								// only if there is text in the buffer
					if (allowEmptyLines || this->mData.input->currentLine.length() > 0) {

						// Set return to mark that a new line was recorded
						enteredNewLine = true;

						// Mark the latest line for the caller
						latestLine = this->mData.input->currentLine;

						// push back the string
						//this->mData.display->textRecord.push_back(latestLine);

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
				case VK_SPACE:
					if (!this->mData.input->currentLine.empty()) {
						this->mData.input->currentLine.push_back((char)MapVirtualKey(i, MAPVK_VK_TO_CHAR));
					}
					break;
					// These should not have an input effect
				case VK_TAB:
				case VK_SHIFT:
				case VK_LSHIFT:
				case VK_RSHIFT:
				case VK_CONTROL:
				case VK_F1:
				case VK_F2:
				case VK_F3:
				case VK_F4:
				case VK_F5:
				case VK_F6:
				case VK_F7:
				case VK_F8:
				case VK_F9:
				case VK_F10:
				case VK_F11:
				case VK_F12:
				case VK_F13:
				case VK_F14:
				case VK_F15:
				case VK_F16:
				case VK_F17:
				case VK_F18:
				case VK_F19:
				case VK_F20:
				case VK_F21:
				case VK_F22:
				case VK_F23:
				case VK_F24:
				case VK_LBUTTON:
				case VK_RBUTTON:
				case VK_LCONTROL:
				case VK_RCONTROL:
					break;
					// Handle all remaining cases (letters and specials)
				default:
					//this->mData.display->textRecord.push_back(std::string());
					// get the character from the virtual key index
					char character = (char)MapVirtualKey(i, MAPVK_VK_TO_CHAR);
					// handle upper/lower case letters: check to see if the shift key is down
					if (!(current[VK_SHIFT] || current[VK_LSHIFT] || current[VK_RSHIFT] || this->mData.input->isCaps)) {
						character = tolower(character);
						
					}
					else
					{
						if (i == '1')
						{
							character = '!';
						}
						else if (i == '2')
						{
							character = '@';
						}
						else if (i == '3')
						{
							character = '#';
						}
						else if (i == '4')
						{
							character = '$';
						}
						else if (i == '5')
						{
							character = '%';
						}
						else if (i == '6')
						{
							character = '^';
						}
						else if (i == '7')
						{
							character = '&';
						}
						else if (i == '8')
						{
							character = '*';
						}
						else if (i == '9')
						{
							character = '(';
						}
						else if (i == '0')
						{
							character = ')';
						}
					}
					// push the character onto the text buffer
					this->mData.input->currentLine.push_back(character);
					break;
			}
		}
	}

	return enteredNewLine;
}

void StateApplication::renderConsole() {
	// Clear the screen
	this->console()->setCursorPosition(0, 0);

	// Get the max lines and columns for the window
	// TODO: Put this in the state
	const int totalLines = 35;
	const int maxColumns = 90;

	// Figure out how many lines can be used for displaying text records
	// one line must be used for user text
	const int totalLinesRecord = totalLines - 1; // 1 for current text

												 // Get the text record from the state data
	std::vector<std::string> textRecord = this->mData.display->textRecord;
	const int textRecordCount = (int)textRecord.size();
	// get the first index of the recorded lines to render
	int textRecordStart = textRecordCount - totalLinesRecord;
	textRecordStart = textRecordStart < 0 ? 0 : textRecordStart;

	for (int line = textRecordStart; line < textRecordCount; line++) {
		std::string text = textRecord[line];
		size_t length = text.length();
		// WARNING: spaceCount is a generative for loop. Move to text enqueue?
		std::string postSpaces = this->console()->spaceCount((int)max(0, maxColumns - length));
		std::cout << text << postSpaces << '\n';
	}

	// Find out how many lines are remaining (these need to be populated via spaces)
	int linesRemaining = totalLinesRecord - textRecordCount;
	linesRemaining = linesRemaining < 0 ? 0 : linesRemaining;

	// Fill empty space with extra spaces and new lines
	// this "pushes" the user's text to the bottom of the screen
	for (int line = 0; line < linesRemaining; line++) {
		std::cout << this->console()->spaceCount(maxColumns) << "\n";
	}

	// Render text incoming from user
	std::string currentText = '"' + this->mData.input->currentLine + '"';
	std::string postSpaces = this->console()->spaceCount((int)max(0, maxColumns - currentText.length()));
	std::cout << "> " << currentText << postSpaces;
}

/* Author: Dustin Yost
Pushes a text message to the record of messages to display
*/
void StateApplication::pushMessage(const std::string &msg) {
	// Push the message into the buffer
	this->mData.display->textRecord.push_back(std::string(msg.c_str()));
}
