#include "Game\State\StateLobby.h"

#include <Windows.h>
#include <iostream>

#include "Game\State\StateChatroom.h"

void StateLobby::updateNetwork() {
	// STUB: no network checks while in lobby
}

void StateLobby::onEnterFrom(StateApplication *previous) {
	StateApplication::onEnterFrom(previous);
	// prep the state to be in the login phase
	mPhase = EnumLoginPhase::NETWORK_TYPE;
	this->mData.input->textRecord.push_back("(C) or (S)erver?");
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
				this->mData.input->textRecord.push_back("Server Port:");
				
				break;
			case EnumLoginPhase::NETWORK_PORT: // Client and Server
				// Log user input
				this->mData.network->networkInfo.port = std::stoi(latestLine);

				// Prompt user for next info part
				if (this->mData.network->isServer) {
					mPhase = EnumLoginPhase::MAX_CLIENTS;
					this->mData.input->textRecord.push_back("Max Clients:");
				}
				else {
					mPhase = EnumLoginPhase::ADDRESS;
					this->mData.input->textRecord.push_back("Enter server IP or hit enter for 127.0.0.1...");
				}

				break;
			case EnumLoginPhase::ADDRESS: // Client
				// Log user input
				strcpy(this->mData.network->networkInfo.serverAddress, latestLine.length() == 0 ? "127.0.0.1" : latestLine.c_str());

				// Startup the server
				this->queueStateChatroom();

				break;
			case EnumLoginPhase::MAX_CLIENTS: // Server
				// Log user input
				this->mData.network->networkInfo.maxClients = std::stoi(latestLine);

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
	// Clear the screen
	this->console()->setCursorPosition(0, 0);
	
	// Get the max lines and columns for the window
	// TODO: Put this in the state
	const int totalLines = 35;
	const int maxColumns = 50;

	// Figure out how many lines can be used for displaying text records
	// one line must be used for user text
	const int totalLinesRecord = totalLines - 1; // 1 for current text

	// Get the text record from the state data
	std::vector<std::string> textRecord = this->mData.input->textRecord;
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
	std::string currentText = this->mData.input->currentLine;
	std::string postSpaces = this->console()->spaceCount((int)max(0, maxColumns - currentText.length()));
	std::cout << "> " << currentText << postSpaces;
}

/* Author: Dustin Yost
Handles updates from the input state and handling how those strings are cached
*/
bool StateLobby::updateForInput(std::string &latestLine, bool allowEmptyLines) {
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
						this->mData.input->textRecord.push_back(latestLine);

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

/* Author: Dustin Yost
Queues the chatroom state
*/
void StateLobby::queueStateChatroom() {
	this->mNext = new StateChatroom();
}
