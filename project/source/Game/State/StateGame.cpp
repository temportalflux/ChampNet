#include "Game\State\StateGame.h"

#include "Game\State\StateGameLocal.h"
#include "Game\State\StateGameNetwork.h"
#include "Game\State\StateLobby.h"

StateGame::StateGame() {
	// mBoardState = array; created already b/c object
	for (int slot = 0; slot < BOARD_SLOTS; slot++) {
		mBoardState[slot] = PlayerIdentifier::NONE;
	}
	mCurrentPlayer = PlayerIdentifier::PLAYER_1;
}

StateGame::~StateGame() {

}

void StateGame::queueNextGameState() {
	this->mNext = new StateLobby();
}

void StateGame::onEnterFrom(StateApplication *previous) {

}

void StateGame::updateGame() {

}

void StateGame::render() {

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
