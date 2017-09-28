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

	mGameCounter = -1;


	//SetConsoleTitle("Local Game");
}

StateGame::~StateGame() {

}

void StateGame::queueNextGameState() {
	this->mNext = new StateLobby();
}

void StateGame::onEnterFrom(StateApplication *previous) {
	StateApplication::onEnterFrom(previous);
	
	this->startNewGame();

	mData.input->resetInput();
}

void StateGame::startNewGame() {
	mLastWinner = mWinner;
	mWinner = NONE;

	mGameCounter++;

	for (int i = 0; i < BOARD_SLOTS; ++i)
		mBoardState[i] = NONE;

	// Set the current player
	mCurrentPlayer = mGameCounter % 2 == 0 ? PLAYER_1 : PLAYER_2;

	mDrawBoardFlag = true;
}

/* Author: Jake Ruth
*  Worked on handleing the input
*/
void StateGame::updateGame() {

	bool *current = this->mData.input->keyboard;
	bool *previous = this->mData.input->previous;

	for (int i = 0; i < StateInput::SIZE_KEYBOARD; i++)
	{
		if (current[i] && !previous[i])
		{
			if (i == VK_ESCAPE)
			{
				this->queueNextGameState();
			}

			if (mWinner == NONE)
			{
				switch (i) // 
				{
				case VK_LEFT: // Move the Selection Cursor

					if ((mSelectionIndex - 1) % 3 != 2 && mSelectionIndex - 1 >= 0)
					{
						mSelectionIndex -= 1;
					}

					break;
				case VK_UP: // Move the Selection Cursor

					if (mSelectionIndex - 3 >= 0)
					{
						mSelectionIndex -= 3;
					}
					

					break;
				case VK_RIGHT: // Move the Selection Cursor

					if ((mSelectionIndex + 1) % 3 != 0)
					{
						mSelectionIndex += 1;
					}

					break;
				case VK_DOWN: // Move the Selection Cursor

					if (mSelectionIndex + 3 < BOARD_SLOTS)
					{
						mSelectionIndex += 3;
					}

					break;
				case VK_RETURN: // Place a move

					// Chack to see if the move is a valid spot
					if (this->validate(mSelectionIndex, mCurrentPlayer))
					{
						// Set the flag to redraw the placed X's and O's
						mUpdateBoardFlag = true;
						mUpdateSelectionFlag = true;

						// Set to see if there is a winner
						// return is NONE if there is no winner
						mWinner = this->commitMove(mSelectionIndex, mCurrentPlayer);
					}

					break;
				default:
					break;
				}
			}
			else // If the game is at the play again screen
			{
				switch (i)
				{
				case VK_LEFT: // Move the Selection Cursor
				case VK_RIGHT: 
					mSelectionIndex = (mSelectionIndex + 1) % 2;
					mUpdateSelectionFlag = true;
					break;
				case VK_RETURN: // Handle the selection

					if(mSelectionIndex % 2 == 0) // On yes
					{
						this->startNewGame();
					}
					else // On no
					{
						this->queueNextGameState();
					}

					break;
				default: 
					break;
				}
			}
		}
	}
}

/* Author: Jake Ruth
 * Worked on the render function
 */
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

	if (mDrawBoardFlag) // Flag to redraw the board
	{
		mDrawBoardFlag = false;

		// flag the rest of the board
		mUpdateBoardFlag = true;
		mUpdateSelectionFlag = true;
		mWinInfoFlag = true;
		
		// initialize variables
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD count;
		COORD pos;
		const DWORD numBytes = 1;
		char buffer[numBytes];

		// fill the entire screen with ' '
		GetConsoleScreenBufferInfo(mStdHandle, &csbi);
		if (!FillConsoleOutputCharacter(mStdHandle, static_cast<TCHAR>(' '), csbi.dwSize.X * csbi.dwSize.Y, { 0, 0 }, &count))
			return;

		// Draw Board
		for (int y = 0; y < 11; ++y)
		{
			pos.Y = y + mOffSet.Y;
			for (int x = 0; x < 11; x++)
			{
				pos.X = x + mOffSet.X;

				buffer[0] = ' ';

				if (x % 4 == 3) // is part of the verticle pipes
					buffer[0] = '|';
				if (y % 4 == 3) // is part of the horizontal pipes
					buffer[0] = '-';
				if (x % 4 == 3 && y % 4 == 3) // is part of both the horizontal and verticle pipes
					buffer[0] = '+';

				// Write out to the console the buffer
				SetConsoleCursorPosition(mStdHandle, pos);
				WriteFile(mStdHandle, buffer, numBytes, nullptr, nullptr);
			}
		}
	}

	if (mUpdateBoardFlag) // flag to redraw the placed moves
	{
		mUpdateBoardFlag = false;

		// initialize variables
		COORD pos;
		const DWORD numBytes = 1;
		char buffer[numBytes];

		// Fill in mBoardState
		const COORD firstCenter = { 1,1 };
		for (int i = 0; i < BOARD_SLOTS; ++i)
		{
			// Convert 1D index to 2D coordinate
			pos.X = mOffSet.X + firstCenter.X + (i % 3) * 4;
			pos.Y = mOffSet.Y + firstCenter.Y + (i / 3) * 4;

			buffer[0] = (mBoardState[i]);

			// Write out to the console the buffer
			SetConsoleCursorPosition(mStdHandle, pos);
			WriteFile(mStdHandle, buffer, numBytes, nullptr, nullptr);
		}

		// move the cursor location to display the current player
		pos = { 14 + mOffSet.X, 1 + mOffSet.Y };

		char turn[25] = "The Current Player is: *";
		turn[23] = mCurrentPlayer;
		
		// Write out to the console the buffer
		SetConsoleCursorPosition(mStdHandle, pos);
		WriteFile(mStdHandle, turn, 25, nullptr, nullptr);

		// if there is a winner, display the winner
		if(mWinner != NONE)
		{
			char winner[15] = "~*~ WINNER ~*~";
			pos = { 18 + mOffSet.X, 5 + mOffSet.Y };

			// Write out to the console the buffer
			SetConsoleCursorPosition(mStdHandle, pos);
			WriteFile(mStdHandle, winner, 15, nullptr, nullptr);
		}
	}

	if(mUpdateSelectionFlag)
	{
		mUpdateSelectionFlag = false;
		COORD pos;
		const DWORD numBytes = 1;
		char buffer[numBytes];

		if (mWinner == NONE)
		{
			// initialize variables
			const COORD firstCenter = { 1,1 };
			COORD boardSlotPosition;

			for (int i = 0; i < BOARD_SLOTS; ++i)
			{
				// Convert 1D index to 2D coordinate
				boardSlotPosition.X = mOffSet.X + firstCenter.X + (i % 3) * 4;
				boardSlotPosition.Y = mOffSet.Y + firstCenter.Y + (i / 3) * 4;

				// Detirme what char to display around the board slot
				buffer[0] = i == mSelectionIndex ? '*' : ' ';

				// Place that char in every spot, except the center
				for (int x = -1; x <= 1; ++x)
				{
					pos.X = boardSlotPosition.X + x;
					for (int y = -1; y <= 1; ++y)
					{
						pos.Y = boardSlotPosition.Y + y;
						if (x == 0 && y == 0) // Skip the center
							continue;

						// Write out to the console the buffer
						SetConsoleCursorPosition(mStdHandle, pos);
						WriteFile(mStdHandle, buffer, numBytes, nullptr, nullptr);
					}
				}
			}
		}
		else
		{
			pos = { 5 + mOffSet.X, 13 + mOffSet.Y };
			const COORD posYes = { pos.X + 2, pos.Y + 3 };
			const COORD posNO = { posYes.X + 6, posYes.Y };

			SetConsoleCursorPosition(mStdHandle, pos);
			WriteFile(mStdHandle, "Play Again?", 12, nullptr, nullptr);

			SetConsoleCursorPosition(mStdHandle, posYes);
			WriteFile(mStdHandle, "Y     N", 7, nullptr, nullptr);

			for (int x = -1; x <= 1; ++x)
			{
				pos.X = posYes.X + x;
				for (int y = -1; y <= 1; ++y)
				{
					pos.Y = posYes.Y + y;
					if (x == 0 && y == 0) // Skip the center
						continue;

					buffer[0] = mSelectionIndex % 2 == 0 ? '*' : ' ';

					// Write out to the console the buffer
					SetConsoleCursorPosition(mStdHandle, pos);
					WriteFile(mStdHandle, buffer, numBytes, nullptr, nullptr);
				}
			}

			for (int x = -1; x <= 1; ++x)
			{
				pos.X = posNO.X + x;
				for (int y = -1; y <= 1; ++y)
				{
					pos.Y = posNO.Y + y;
					if (x == 0 && y == 0) // Skip the center
						continue;

					buffer[0] = mSelectionIndex % 2 == 1 ? '*' : ' ';

					// Write out to the console the buffer
					SetConsoleCursorPosition(mStdHandle, pos);
					WriteFile(mStdHandle, buffer, numBytes, nullptr, nullptr);
				}
			}
		}
	}

	if(mWinInfoFlag)
	{
		mWinInfoFlag = false;

		/*COORD pos;

		pos = { 14 + mOffSet.X, 7 + mOffSet.Y };

		char *bufferX = "Player X wins: ";
		char *bufferY = "Player Y wins: ";*/

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
	mSelectionIndex = 4;

	mUpdateSelectionFlag = true; // Set the flag to draw the selection

	if (winner == NONE) {
		mCurrentPlayer = player == PLAYER_1 ? PLAYER_2 : PLAYER_1;
	} 
	else if (winner == PLAYER_1) {
		mPlayer1WinCount++;
		if (mLastWinner == winner)
			mWinStreak++;
		else
			mWinStreak = 0;
	}
	else if (winner == PLAYER_2) {
		mPlayer2WinCount++;
		if (mLastWinner == winner)
			mWinStreak++;
		else
			mWinStreak = 0;
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
	int offset = row * BOARD_N;
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
		if (slot % 2 == 0 || slot == 4) {
			
			// Reset vars
			playerID = NONE;
			valid = true;
			// Iterate over diagonal
			for (i = 2; i <= 6; i += 2) {
				// If first slot, set the ID
				if (i == 2) playerID = mBoardState[i];
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
