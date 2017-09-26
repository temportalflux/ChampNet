#include "Game\State\StateGame.h"

#include "Game\State\StateGameLocal.h"
#include "Game\State\StateGameNetwork.h"
#include "Game\State\StateLobby.h"
#include <iostream>

StateGame::StateGame() {
	// mBoardState = array; created already b/c object
	for (int slot = 0; slot < BOARD_SLOTS; slot++) {
		mBoardState[slot] = PlayerIdentifier::NONE;
	}
	mCurrentPlayer = PlayerIdentifier::PLAYER_1;

	mDrawBoardFlag = true;

}

StateGame::~StateGame() {

}

void StateGame::queueNextGameState() {
	this->mNext = new StateLobby();
}

void StateGame::onEnterFrom(StateApplication *previous) {

}

void StateGame::updateGame() {

	bool *current = this->mData.input->keyboard;
	bool *previous = this->mData.input->previous;

	for (int i = 0; i < StateInput::SIZE_KEYBOARD; ++i)
	{
		if(current[i] && !previous[i])
		{
			//if(i == 37)
			if (MapVirtualKey(i, MAPVK_VK_TO_CHAR) == 37) // Left
			{
				if (mSelectionIndex - 1 <= 0)
					mSelectionIndex -= 1;

				mUpdateSelectionFlag = true;
			}

			if (MapVirtualKey(i, MAPVK_VK_TO_CHAR) == 38) // Up
			{
				if (mSelectionIndex + 3 < BOARD_SLOTS)
					mSelectionIndex += 3;

				mUpdateSelectionFlag = true;
			}

			if ((char)MapVirtualKey(i, MAPVK_VK_TO_CHAR) == 'D') // Right
			{
				if (mSelectionIndex + 1 < BOARD_SLOTS)
					mSelectionIndex += 1;

				mUpdateSelectionFlag = true;
			}

			if (MapVirtualKey(i, MAPVK_VK_TO_CHAR) == 40) // Down
			{
				if (mSelectionIndex - 3 <= 0)
					mSelectionIndex -= 3;

				mUpdateSelectionFlag = true;
			}
		}
	}
	
	if (current[VK_ESCAPE])
	{
		std::cout << "here";
	}
}

void StateGame::render() {
	/*
	// For Testing Drawing
	mBoardState[0] = PLAYER_1;
	mBoardState[1] = PLAYER_2;
	mBoardState[2] = PLAYER_1;
	mBoardState[3] = PLAYER_2;
	mBoardState[4] = PLAYER_1;
	mBoardState[5] = PLAYER_2;
	mBoardState[6] = PLAYER_1;
	mBoardState[7] = PLAYER_2;
	mBoardState[8] = PLAYER_1;

	mSelectionIndex = 3;
	//*/

	
	if (mDrawBoardFlag)
	{

		mDrawBoardFlag = false;

		mUpdateBoardFlag = true;
		mUpdateSelectionFlag = true;

		const HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD count;
		COORD pos;
		const COORD offSet = { 3,3 };
		const DWORD numBytes = 1;
		char buffer[3];

		GetConsoleScreenBufferInfo(stdHandle, &csbi);
		if (!FillConsoleOutputCharacter(stdHandle, static_cast<TCHAR>(' '), csbi.dwSize.X * csbi.dwSize.Y, { 0, 0 }, &count))
			return;

		// Draw Board
		for (int y = 0; y < 11; ++y)
		{
			pos.Y = y + offSet.Y;
			for (int x = 0; x < 11; x++)
			{
				pos.X = x + offSet.X;

				buffer[0] = ' ';

				if (x % 4 == 3)
					buffer[0] = '|';
				if (y % 4 == 3)
					buffer[0] = '-';
				if (x % 4 == 3 && y % 4 == 3)
					buffer[0] = '+';

				SetConsoleCursorPosition(stdHandle, pos);
				WriteFile(stdHandle, buffer, numBytes, nullptr, nullptr);
			}
		}
	}

	if (mUpdateBoardFlag)
	{
		mUpdateBoardFlag = false;

		const HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD count;
		COORD pos;
		const COORD offSet = { 3,3 };
		const DWORD numBytes = 1;
		char buffer[numBytes];

		// Fill in mBoardState
		const COORD firstCenter = { 1,1 };
		for (int i = 0; i < BOARD_SLOTS; ++i)
		{

			pos.X = offSet.X + firstCenter.X + (i % 3) * 4;
			pos.Y = offSet.Y + firstCenter.Y + (i / 3) * 4;

			buffer[0] = (mBoardState[i]);

			SetConsoleCursorPosition(stdHandle, pos);
			WriteFile(stdHandle, buffer, numBytes, nullptr, nullptr);
		}
	}

	if(mUpdateSelectionFlag)
	{
		mUpdateSelectionFlag = false;

		const HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD count;
		COORD pos;
		const COORD offSet = { 3,3 };
		const DWORD numBytes = 1;
		char buffer[numBytes];
		const COORD firstCenter = { 1,1 };

		// Show Selection
		COORD selectionPos;
		selectionPos.X = offSet.X + firstCenter.X + (mSelectionIndex % 3) * 4;
		selectionPos.Y = offSet.Y + firstCenter.Y + (mSelectionIndex / 3) * 4;

		for (int x = -1; x <= 1; ++x)
		{
			pos.X = selectionPos.X + x;
			for (int y = -1; y <= 1; ++y)
			{
				pos.Y = selectionPos.Y + y;
				if (x == 0 && y == 0)
					continue;

				buffer[0] = '*';

				SetConsoleCursorPosition(stdHandle, pos);
				WriteFile(stdHandle, buffer, numBytes, nullptr, nullptr);
			}
		}
	}
}

/* Author: Dustin Yost
	Validate the player's selection of a slot
*/
bool StateGame::validate(int slot, PlayerIdentifier player) {
	// False if:
	// - invalid slot
	// - slot not empty
	return (slot >= 0 && slot < BOARD_SLOTS) &&
		mBoardState[slot] == PlayerIdentifier::NONE;
}

/* Author: Dustin Yost
	Sets the player's icon in a specfied slot,
	and sets the next player.
*/
StateGame::PlayerIdentifier StateGame::commitMove(int slot, PlayerIdentifier player) {
	mBoardState[slot] = player;
	PlayerIdentifier winner = this->checkWinstate(slot);
	if (winner == NONE) {
		mCurrentPlayer = player == PLAYER_1 ? PLAYER_2 : PLAYER_1;
	}
	return winner;
}

/* Author: Dustin Yost
	Checks to see if either player has won
*/
StateGame::PlayerIdentifier StateGame::checkWinstate(int slot) {

	PlayerIdentifier playerID;
	int i, j;
	bool valid;

	/*
		    0  1  2
		  +---------
		0 | 0  1  2
		1 | 3  4  5
		2 | 6  7  8
		if slot = 0, check [0, 2], {0, 3, 6}, and {0, 4, 8}

		n = 3
		row = slot / n, col = slot % n
		HORIZONTAL: i = slot - (row * n) => {i, i + 1, i + 2}
		VERTICAL: {col + 0(n), col + 1(n), col + 2(n)}
		DIAGONAL: will only happen to 0, 2, 4, 6, 8 (EVEN NUMBERS)
			HARDCODED FOR THIS SCOPE
	*/

	int row = slot / BOARD_N, col = slot % BOARD_N;

	// ~~~~~~~~~~~~~~ Check Horizontal ~~~~~~~~~~~~~~
	// Reset vars
	playerID = NONE;
	valid = true;
	// Calculate offset for the row
	int offset = slot - row * BOARD_N;
	// Iterate over row
	for (i = 0; i < 3; i++) {
		// Find each slot
		j = i + offset;
		// If first slot, set the ID
		if (i == 0) playerID = mBoardState[j];
		// else check for matching ID
		else if (playerID != mBoardState[j]) {
			// If not matching, row is not valid for win
			valid = false;
			break;
		}
	}
	// If the row was valid, return winstate occured for which player
	if (valid) {
		return playerID;
	}

	// ~~~~~~~~~~~~~~ Check Vertical ~~~~~~~~~~~~~~
	// Reset vars
	playerID = NONE;
	valid = true;
	// Iterate over column
	for (i = 0; i < 3; i++) {
		// Find each slot
		j = col + i * BOARD_N;
		// If first slot, set the ID
		if (i == 0) playerID = mBoardState[j];
		// else check for matching ID
		else if (playerID != mBoardState[j]) {
			// If not matching, row is not valid for win
			valid = false;
			break;
		}
	}
	// If the column was valid, return winstate occured for which player
	if (valid) {
		return playerID;
	}

	// ~~~~~~~~~~~~~~ Check Diagonal ~~~~~~~~~~~~~~
	// Diagonals are only checked for even numbers
	if (slot % 2 == 0) {
		
		// Top Left -> Bottom Right = 0, 4, 8
		if (slot % 4 == 0) {

			// Reset vars
			playerID = NONE;
			valid = true;
			// Iterate over diagonal
			for (i = 0; i < 9; i += 4) {
				// If first slot, set the ID
				if (i == 0) playerID = mBoardState[i];
				// else check for matching ID
				else if (playerID != mBoardState[i]) {
					// If not matching, set is not valid for win
					valid = false;
					break;
				}
			}
			// If valid, return winstate occured for which player
			if (valid) {
				return playerID;
			}

		}

		// Top Right -> Bottom Left = 2, 4, 6
		if (slot % 4 != 0 || slot == 4) {
			
			// Reset vars
			playerID = NONE;
			valid = true;
			// Iterate over diagonal
			for (i = 2; i <= 6; i += 2) {
				// If first slot, set the ID
				if (i == 0) playerID = mBoardState[i];
				// else check for matching ID
				else if (playerID != mBoardState[i]) {
					// If not matching, set is not valid for win
					valid = false;
					break;
				}
			}
			// If valid, return winstate occured for which player
			if (valid) {
				return playerID;
			}

		}

	}

	return NONE;
}
