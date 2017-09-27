#pragma once

#pragma once

#include "Game\State\StateApplication.h"

// Author: Dustin Yost
class StateGame abstract : public StateApplication {

private:

	static const int BOARD_N = 3;
	static const int BOARD_SLOTS = BOARD_N * BOARD_N;

	/* Author: Dustin Yost
		All the possible identifiers in the game board
	*/
	enum PlayerIdentifier {
		NONE = ' ',
		PLAYER_1 = 'X',
		PLAYER_2 = 'O',
	};

	/* Author: Dustin Yost
		An array of 9 positions on the game board
	*/
	PlayerIdentifier mBoardState[BOARD_SLOTS];
	PlayerIdentifier mCurrentPlayer;

	/* Author: Jake Ruth
	 * Current position of selection
	 */
	int mSelectionIndex;

	/* Author: Jake Ruth
	 * Previous position of selection
	 */
	int mPreviousSelectionIndex;

	/* Author: Jake Ruth
	 * Draw Game Board flag
	 */
	bool mDrawBoardFlag;

	/* Author: Jake Ruth
	* Draw Game Board flag
	*/
	bool mUpdateBoardFlag;

	/* Author: Jake Ruth
	* Draw Game Board flag
	*/
	bool mUpdateSelectionFlag;

	/* Author: Jake Ruth
	 * Used to determine if there is a winner
	 */
	bool mIsWinner;

	virtual void queueNextGameState() override;

	virtual void onEnterFrom(StateApplication *previous);
	void updateGame();
	void render();



public:

	StateGame();
	virtual ~StateGame();

	/* Author: Dustin Yost
		Validate the player's selection of a slot
	*/
	bool validate(int slot, PlayerIdentifier player);

	/* Author: Dustin Yost
		Sets the player's icon in a specfied slot,
		and sets the next player.
		Returns the player that won, NONE if no one has won yet
	*/
	virtual PlayerIdentifier commitMove(int slot, PlayerIdentifier player);

	/* Author: Dustin Yost
		Checks to see if either player has won.
		Returns the Player ID of the winner, NONE if there was none;
	*/
	PlayerIdentifier checkWinstate(int slot);

};
