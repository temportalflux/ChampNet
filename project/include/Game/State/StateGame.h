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

	virtual void queueNextGameState() override;

	virtual void onEnterFrom(StateApplication *previous);
	void updateGame();
	void render();

public:

	StateGame();
	virtual ~StateGame();

	static StateApplication* create(bool isLocalGame);

};
