#ifndef CHAMPNET_SERVER_GAME_H
#define CHAMPNET_SERVER_GAME_H

class StateApplication;

/** Author: Dustin Yost
 * A base class for all update loop based game information (runs the game)
 */
class Game
{

protected:

	// The information state - handles all persistent data
	StateApplication *mpState, *mpNext;

public:

	static Game *gpGame;

	Game(StateApplication *state);
	Game();
	virtual ~Game();

	const bool isRunning() const;

	/** Author: Dustin Yost
	 * Handles all gameloop updates
	 */
	void update();

	void queueState(StateApplication *nextState);
	void goToNextState();

};

#endif // CHAMPNET_SERVER_GAME_H