#include "Game\State\StateLobby.h"
#include <Windows.h>
#include <iostream>

void StateLobby::updateNetwork() {

}

void StateLobby::updateGame() {
	// Cache keyboard state
	bool *current = this->mData.input->keyboard;
	bool *previous = this->mData.input->previous;

	// Handle states
	if (current[VK_ESCAPE] && !previous[VK_ESCAPE]) {
		this->mRunning = false;
	}

}

void StateLobby::render() {
	system("cls");

}
