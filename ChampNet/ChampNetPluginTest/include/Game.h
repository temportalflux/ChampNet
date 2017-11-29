#ifndef CHAMPNET_SERVER_GAME_H
#define CHAMPNET_SERVER_GAME_H

class StateApplication;
struct StateData;

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

	// Returns if the game is presently in a running state
	const bool isRunning() const;

	// Returns the system data
	StateData* getData() const;

	/** Author: Dustin Yost
	 * Handles all gameloop updates
	 */
	void update();

	void queueState(StateApplication *nextState);

	void goToNextState();

};

#endif // CHAMPNET_SERVER_GAME_H