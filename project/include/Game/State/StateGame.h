#pragma once

#pragma once

#include "Game\State\StateApplication.h"

// Author: Dustin Yost
class StateGame abstract : public StateApplication {

protected:

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

private:

	/* Author: Dustin Yost
		An array of 9 positions on the game board
	*/
	PlayerIdentifier mBoardState[BOARD_SLOTS];
	

	/* Author: Jake Ruth
	 * Current position of selection
	 */
	int mSelectionIndex;

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
	 * Set the offset of the board
	 */
	const COORD mOffSet = { 3,3 };

	/* Author: Jake Ruth
	 * Use to get the console's output handle
	 */
	const HANDLE mStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	/* Author: Jake Ruth
	 * Use to count the number of games
	 */
	int mGameCounter;

	/* Author: Jake Ruth
	 * Use to count the number of wins player 1 has
	 */
	int mPlayer1WinCount;

	/* Author: jake Ruth
	 * Use to count the number of wins player 2 has
	 */
	int mPlayer2WinCount;

	/* Author: Jake Ruht
	 * Use to store the last winner
	 */
	PlayerIdentifier mLastWinner;

	/* Author: Jake Ruth
	 * Use to store the win streak
	 */
	int mWinStreak;

	/* Author: Jake Ruth
	 * Draw Win info flag
	 */
	bool mWinInfoFlag;

	virtual void onEnterFrom(StateApplication *previous);
	void updateGame();
	void render();

protected:

	/* Author: Jake Ruth
	* Used to determine if there is a winner
	* if NONE then no winner
	*/
	PlayerIdentifier mWinner;

	PlayerIdentifier mCurrentPlayer;

	/* Author: jake Ruth
	 * used to set play again
	 */
	virtual void setPlayAgain();

	virtual void queueNextGameState() override;

public:

	StateGame();
	virtual ~StateGame();

	void startNewGame();

	/* Author: Dustin Yost
		Validate the player's selection of a slot
	*/
	virtual bool validate(int slot, PlayerIdentifier player);

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
