#ifndef CHAMPNET_SERVER_GAME_H
#define CHAMPNET_SERVER_GAME_H

#include <string>

struct StateInput
{
	// The max size of the keyboard arrays
	static const unsigned int SIZE_KEYBOARD = 256;

	// Two arrays to keep track of previous and current keyboard states
	char keyboard[SIZE_KEYBOARD];
	char previous[SIZE_KEYBOARD];
	// If the input is currently treated as the CAPS lock being on
	bool isCaps;
	// If empty lines are enabled on ENTER
	bool allowEmptyLines;
	// The current line of text (before pressing ENTER)
	std::string currentLine;

};

struct StateConsole
{
	// A pointer to the console window
	void *consoleWindow;
};

struct StateNetwork
{

	// The port the network is connected to
	unsigned int port;

};

struct StateGame
{
	StateInput mInput;
	StateConsole mConsole;
	StateNetwork mNetwork;
};

/** Author: Dustin Yost
 * A base class for all update loop based game information (runs the game)
 */
class Game
{

private:

	// Should the loop continue
	bool mIsRunning;

protected:

	// The information state - handles all persistent data
	StateGame *mpState;

	/** Author: Dustin Yost
	 * Collects all input changes
	 */
	virtual void updateInput();
	
	/** Updates the network | STUB */
	virtual void updateNetwork();
	
	/** Author: Dustin Yost
	 * Handles all game updates and responses to input/network updates
	 */
	virtual void updateGame();

	/** Author: Dustin Yost
	 * Updates states according to input updates
	 */
	virtual void updateGameForInput(const char *current, const char *previous);

	/** Author: Dustin Yost
	 * Called when a key is marked as down this update
	 */
	virtual void onKeyDown(int i);
	
	/** Renders the game to the display | STUB */
	virtual void render();

public:

	Game();
	virtual ~Game();

	inline const bool isRunning() const { return mIsRunning; }

	/** Author: Dustin Yost
	 * Handles all gameloop updates
	 */
	void update();

};

#endif // CHAMPNET_SERVER_GAME_H