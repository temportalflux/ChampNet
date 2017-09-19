#include "Game\State\StateLobby.h"

#include <Windows.h>
#include <iostream>

void StateLobby::updateNetwork() {

}

void StateLobby::updateGame() {
	// Cache keyboard state
	bool *current = this->mData.input->keyboard;
	bool *previous = this->mData.input->previous;
	
	for (int i = 0; i < StateInput::SIZE_KEYBOARD; i++) {
		if (current[i] && !previous[i]) {
			switch (i) {
				case VK_ESCAPE:
					this->mRunning = false;
					break;
				case VK_RETURN:
					if (this->mData.input->currentLine.length() > 0) {
						this->mData.input->textRecord.push_back(this->mData.input->currentLine);
						this->mData.input->currentLine = "";
					}
					break;
				case VK_BACK:
					{
						std::string str = this->mData.input->currentLine;
						if (str.length() > 0) {
							this->mData.input->currentLine = str.substr(0, str.length() - 1);
						}
					}
					break;
				case VK_CAPITAL:
					this->mData.input->isCaps = !this->mData.input->isCaps;
					break;
				case VK_SHIFT: break;
				case VK_LSHIFT: break;
				case VK_RSHIFT: break;
				case VK_CONTROL: break;
				// Handle all remaining cases (letters and specials)
				default:
					char character = (char)MapVirtualKey(i, MAPVK_VK_TO_CHAR);
					if (!(current[VK_SHIFT] || current[VK_LSHIFT] || current[VK_RSHIFT] || this->mData.input->isCaps)) {
						character = tolower(character);
					}
					this->mData.input->currentLine.push_back(character);
					break;
			}
		}
	}

}

void StateLobby::render() {
	// Clear the screen
	this->console()->setCursorPosition(0, 0);
	
	const int totalLines = 35;
	const int maxColumns = 50;
	const int totalLinesRecord = totalLines - 1; // 1 for current text

	std::vector<std::string> textRecord = this->mData.input->textRecord;
	const size_t textRecordCount = textRecord.size();
	// get the first index of the recorded lines to render
	int textRecordStart = textRecordCount - totalLinesRecord;
	textRecordStart = textRecordStart < 0 ? 0 : textRecordStart;

	int linesRemaining = totalLinesRecord - textRecordCount;
	linesRemaining = linesRemaining < 0 ? 0 : linesRemaining;

	for (int line = textRecordStart; line < textRecordCount; line++) {
		std::string text = textRecord[line];
		int length = text.length();
		// WARNING: spaceCount is a generative for loop. Move to text enqueue?
		std::string postSpaces = this->console()->spaceCount((int)max(0, maxColumns - length));
		std::cout << text << postSpaces << '\n';
	}

	for (int line = 0; line < linesRemaining; line++) {
		std::cout << this->console()->spaceCount(maxColumns) << "\n";
	}

	std::string currentText = this->mData.input->currentLine;
	std::string postSpaces = this->console()->spaceCount((int)max(0, maxColumns - currentText.length()));
	std::cout << "> " << currentText << postSpaces;
}
