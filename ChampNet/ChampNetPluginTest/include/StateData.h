#ifndef CHAMPNET_SERVER_STATE_DATA_H
#define CHAMPNET_SERVER_STATE_DATA_H

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
	// The previous size of the currentLine (-1 when the line is cleared)
	unsigned int lineSizePrevious;

};

struct StateConsole
{

	// A pointer to the console window
	void *consoleWindow;

	// The current cursor position in the console
	int cursorPosX, cursorPosY;

};

struct StateNetwork
{

	// The port the network is connected to
	unsigned int port;

	unsigned int maxClients;

};

struct StateData
{
	StateInput mInput;
	StateConsole mConsole;
	StateNetwork mNetwork;
};

#endif // CHAMPNET_SERVER_STATE_DATA_H