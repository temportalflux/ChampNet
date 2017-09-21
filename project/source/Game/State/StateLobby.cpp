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

#include <Windows.h>

#include "Game\State\StateChatroomClient.h"
#include "Game\State\StateChatroomServer.h"

void StateLobby::updateNetwork() {
	// STUB: no network checks while in lobby
}

void StateLobby::onEnterFrom(StateApplication *previous) {
	StateApplication::onEnterFrom(previous);
	// prep the state to be in the login phase
	mPhase = EnumLoginPhase::NETWORK_TYPE;
	this->mData.display->textRecord.push_back("(C) or (S)erver?");
}

/* Author: Dustin Yost
Handle game updates
*/
void StateLobby::updateGame() {
	
	// varaible to cache the last entry (if a new line was entered) from the recorded text
	std::string latestLine;
	// update for input state changes, will be true if a new line was entered
	if (this->updateForInput(latestLine, mPhase == EnumLoginPhase::ADDRESS)) {
		// new line has been entered

		switch (mPhase) {
			case EnumLoginPhase::NETWORK_TYPE: // Client and Server
				// Log user input
				this->mData.network->isServer = latestLine[0] == 's' || latestLine[0] == 'S';
				
				// Prompt user for next info part
				mPhase = EnumLoginPhase::NETWORK_PORT;
				this->mData.display->textRecord.push_back("Server Port:");
				
				break;
			case EnumLoginPhase::NETWORK_PORT: // Client and Server
				// Log user input
				this->mData.network->networkInfo.port = std::stoi(latestLine);

				// Prompt user for next info part
				if (this->mData.network->isServer) {
					mPhase = EnumLoginPhase::MAX_CLIENTS;
					this->mData.display->textRecord.push_back("Max Clients:");
				}
				else {
					mPhase = EnumLoginPhase::ADDRESS;
					this->mData.display->textRecord.push_back("Enter server IP or hit enter for 127.0.0.1...");
				}

				break;
			case EnumLoginPhase::MAX_CLIENTS: // Server
											  // Log user input
				this->mData.network->networkInfo.maxClients = std::stoi(latestLine);

				this->mData.display->textRecord.clear();

				// Startup the server
				this->queueStateChatroom();

				break;
			case EnumLoginPhase::ADDRESS: // Client
				// Log user input
				strcpy(this->mData.network->networkInfo.serverAddress, latestLine.length() == 0 ? "127.0.0.1" : latestLine.c_str());

				// Prompt user for next info part
				mPhase = EnumLoginPhase::USERNAME;
				this->mData.display->textRecord.push_back("Username:");

				break;
			case EnumLoginPhase::USERNAME: // CLIENT
				// Set username of client
				this->mData.network->networkInfo.username.assign(latestLine);

				this->mData.display->textRecord.clear();

				// Startup the server
				this->queueStateChatroom();

				break;
			default:
				break;
		}

	}

}

/* Author: Dustin Yost
Handle rendering all text to the screen
*/
void StateLobby::render() {
	this->renderConsole();
}

/* Author: Dustin Yost
Queues the chatroom state
*/
void StateLobby::queueStateChatroom() {
	if (this->mData.network->isServer) {
		this->mNext = new StateChatroomServer();
	}
	else {
		this->mNext = new StateChatroomClient();
	}
}
