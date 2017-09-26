#include "Game\State\StateGame.h"

#include "Game\State\StateGameLocal.h"
#include "Game\State\StateGameNetwork.h"
#include "Game\State\StateLobby.h"
#include <iostream>

/* Author: Dustin Yost
	Creates a local/network game state
*/
StateGame* StateGame::create(bool isLocalGame) {
	if (isLocalGame) {
		return new StateGameLocal();
	}
	else {
		return new StateGameNetwork();
	}
}

StateGame::StateGame() {
	// mBoardState = array; created already b/c object
	for (int slot = 0; slot < BOARD_SLOTS; slot++) {
		mBoardState[slot] = PlayerIdentifier::NONE;
	}
	mCurrentPlayer = PlayerIdentifier::PLAYER_1;

	mDrawBoard = true;

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

	if (current[38] && !previous[38])
		mSelectionIndex++;
}

void StateGame::render() {
	if(mDrawBoard)
	{
		
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

		mDrawBoard = false;

		const HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD count;
		COORD pos;
		const COORD offSet = {3,3};
		const DWORD numBytes = 1;
		char buffer[3];

		GetConsoleScreenBufferInfo(stdHandle, &csbi);
		if(!FillConsoleOutputCharacter(stdHandle, static_cast<TCHAR>(' '), csbi.dwSize.X * csbi.dwSize.Y, { 0, 0 }, &count))
			return;

		// Draw Board
		for (int y = 0; y < 11; ++y)
		{
			pos.Y = y + offSet.Y;
			for (int x = 0; x < 11; x++)
			{
				pos.X = x + offSet.X;
				
				buffer[0] = ' ';
				
				if(x % 4 == 3)
					buffer[0] = '|';
				if (y % 4 == 3)
					buffer[0] = '-';
				if (x % 4 == 3 && y % 4 == 3)
					buffer[0] = '+';

				SetConsoleCursorPosition(stdHandle, pos);
				WriteFile(stdHandle, buffer, numBytes, nullptr, nullptr);
			}
		}

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

		/*COORD placeSelection;
		placeSelection.X = offSet.X + firstCenter.X + (mSelectionIndex % 3) * 4;
		placeSelection.Y = offSet.Y + firstCenter.Y + (mSelectionIndex / 3) * 4;

		buffer[0] = 'X';

		SetConsoleCursorPosition(stdHandle, placeSelection);

		WriteFile(stdHandle, buffer, numBytes, nullptr, nullptr);*/

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
