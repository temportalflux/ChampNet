#include "Game\State\StateApplication.h"
#include <cstdlib>
#include <Windows.h>

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

bool StateApplication::hasNext() {
	return mNext != NULL;
}

StateApplication* StateApplication::getNext() {
	return mNext;
}

void StateApplication::onExit() {}

void StateApplication::onEnterFrom(StateApplication *previous) {
	this->mPrevious = previous;
	this->mData = previous->mData;
}

void StateApplication::updateInput() {
	//GetKeyboardState(this->mData.input->keyboard);
	// Gather all keyboard states
	for (int key = 0; key < StateInput::SIZE_KEYBOARD; key++) {
		this->mData.input->previous[key] = this->mData.input->keyboard[key];
		this->mData.input->keyboard[key] = (GetAsyncKeyState(key) & 0x8000) != 0;
	}
}
