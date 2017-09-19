#include "Game\State\StateApplication.h"
#include <cstdlib>
#include <Windows.h>

void StateConsole::setSize(RECT rect) {
	this->size = rect;
}

void StateConsole::showConsoleCursor(bool showFlag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

void StateConsole::setCursorPosition(int x, int y) {
	COORD cur = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
}

std::string StateConsole::spaceCount(int count) {
	std::stringstream s;
	for (int i = 0; i < count; i++) s << ' ';
	return s.str();
}

StateApplication::Data::Data() {
	console = new StateConsole();
	input = new StateInput();
}

StateApplication::Data::~Data() {
	delete console;
	delete input;
}

StateApplication::StateApplication() {
	mNext = NULL;
	mPrevious = NULL;
}

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

void StateApplication::onEnterFrom(StateApplication *previous) {
	this->mPrevious = previous;
	this->mData = previous->mData;
}

void StateApplication::updateInput() {
	byte* mainState = NULL;
	GetKeyboardState(mainState);
	// Gather all keyboard states
	for (int key = 0; key < StateInput::SIZE_KEYBOARD; key++) {
		this->mData.input->previous[key] = this->mData.input->keyboard[key];
		this->mData.input->keyboard[key] = (GetAsyncKeyState(key) & 0x8000) != 0;
	}
}

void StateApplication::setWindow(HWND window) {
	this->mData.console->consoleWindow = window;
}

bool StateApplication::hasNext() {
	return mNext != NULL;
}

StateApplication* StateApplication::getNext() {
	return mNext;
}

StateConsole* StateApplication::console() {
	return mData.console;
}
