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

#include "Game\State\StateLobby.h"

#include "lib\LibWindows.h"

#include "Game\State\StateGameLocal.h"
#include "Game\State\StateConnecting.h"

void StateLobby::updateNetwork() {
	// STUB: no network checks while in lobby
}

void StateLobby::onEnterFrom(StateApplication *previous) {
	StateApplication::onEnterFrom(previous);

	SetConsoleTitle("Lobby");

	this->mData.display->textRecord.clear();
	this->mEscape = false;

	this->mData.input->fillKeyboard();

	mPhase = LobbyPhase::NETWORK_TYPE;
	this->promptPhase(mPhase);
}

/* Author: Dustin Yost
Handle game updates
*/
void StateLobby::updateGame() {
	
	// varaible to cache the last entry (if a new line was entered) from the recorded text
	std::string latestLine;
	// update for input state changes, will be true if a new line was entered
	if (this->updateForInput(latestLine, mPhase == LobbyPhase::ADDRESS)) {
		// new line has been entered
		
		size_t last = this->mData.display->textRecord.size() - 1;
		this->mData.display->textRecord[last] = this->mData.display->textRecord[last] + latestLine;

		this->handlePhaseInput(mPhase, latestLine, mPhase);

		// No next state, so print the next prompt
		if (mNext == NULL) {
			this->promptPhase(mPhase);
		}

	}
	if (this->mEscape) {
		this->mRunning = false;
	}

}

/* Author: Dustin Yost
Handle rendering all text to the screen
*/
void StateLobby::render() {
	this->renderConsole();
}

void StateLobby::handlePhaseInput(LobbyPhase phase, const std::string &line, LobbyPhase &next) {
	switch (phase) {
		case LobbyPhase::NETWORK_TYPE: // ALL
			{
				int input = std::stoi(line) - 1;
				this->mData.network->networkType = (StateNetwork::NetworkType)(input);
				if (this->mData.network->networkType != StateNetwork::LOCAL) {
					next = LobbyPhase::NETWORK_PORT;
				}
				else {
					this->queueNextGameState();
				}
			}
			break;
		case LobbyPhase::NETWORK_PORT: // HOST or PEER
			// Log user input
			this->mData.network->networkInfo.port = std::stoi(line);
			
			if (this->mData.network->networkType == StateNetwork::PEER) {
				// Peer state, need address + port
				next = LobbyPhase::ADDRESS;
			}
			else {
				// Host state, ready for connection
				this->queueNextGameState();
			}

			break;
		case LobbyPhase::ADDRESS: // PEER 
			{
				// Log user input
				bool emptyLine = line.length() == 0;
				if (emptyLine) {
					this->pushMessage("127.0.0.1");
					strcpy(this->mData.network->networkInfo.serverAddress, "127.0.0.1");
				}
				else {
					strcpy(this->mData.network->networkInfo.serverAddress, line.c_str());
				}
				// Peer state, ready for connection
				this->queueNextGameState();
			}
			break;
		default:
			break;
	}
}

void StateLobby::promptPhase(LobbyPhase phase) {
	switch (phase) {
		case LobbyPhase::NETWORK_TYPE: // ALL
			this->pushMessage("Game Types:");
			this->pushMessage("1) Local");
			this->pushMessage("2) Online Host");
			this->pushMessage("3) Online Peer");
			this->pushMessage("Select game type: ");
			break;
		case LobbyPhase::NETWORK_PORT: // HOST orPEER
			this->pushMessage("Host Port: ");
			break;
		case LobbyPhase::ADDRESS: // PEER
			this->pushMessage("Enter host IP or hit enter for 127.0.0.1... ");
			break;
		default:
			break;
	}
}

void StateLobby::queueNextGameState() {
	StateNetwork::NetworkType type = this->mData.network->networkType;
	if (type == StateNetwork::HOST || type == StateNetwork::PEER) {
		// Both should run the connecting state
		mNext = new StateConnecting();
	}
	else {
		// This should run the game locally
		mNext = new StateGameLocal();
	}
}
