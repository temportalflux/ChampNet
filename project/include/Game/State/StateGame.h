#pragma once

#pragma once

#include "Game\State\StateApplication.h"

// Author: Dustin Yost
class StateGame abstract : public StateApplication {

private:

	static const int BOARD_SLOTS = 9;

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
	 * Draw Game Board flag
	 */
	bool mDrawBoard;

	virtual void queueNextGameState() override;

	virtual void onEnterFrom(StateApplication *previous);
	void updateGame();
	void render();



public:

	StateGame();
	virtual ~StateGame();

	/* Author: Dustin Yost
		Creates a local/network game state
	*/
	static StateGame* create(bool isLocalGame);

	/* Author: Dustin Yost
		Validate the player's selection of a slot
	*/
	virtual bool validate(int slot, PlayerIdentifier player);

};
