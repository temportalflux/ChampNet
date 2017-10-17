#include "StateServer.h"

#include "ChampNetPlugin.h"

#include "StateData.h"
#include "Win.h"
#include <iostream>

StateServer::StateServer() : StateApplication()
{
	ChampNetPlugin::Create();
}

StateServer::~StateServer()
{
	this->disconnect();
	ChampNetPlugin::Destroy();
}

void StateServer::onEnterFrom(StateApplication *previous)
{
	StateApplication::onEnterFrom(previous);
	this->start();
}

/** Author: Dustin Yost
* Called when a key is marked as down this update
*/
void StateServer::onKeyDown(int i)
{
	StateApplication::onKeyDown(i);
	char *current = this->mpState->mInput.keyboard;

	// Check to see if the user's input has stopped the game
	if (!this->isRunning())
	{
		this->disconnect();
		return;
	}

	// Update the line size to track the previous size
	this->mpState->mInput.lineSizePrevious = (unsigned int)this->mpState->mInput.currentLine.length();

	switch (i)
	{
		// These should not have an input effect
		case VK_SHIFT:
		case VK_LSHIFT:
		case VK_RSHIFT:
		case VK_CONTROL:
		case VK_F1:
		case VK_F2:
		case VK_F3:
		case VK_F4:
		case VK_F5:
		case VK_F6:
		case VK_F7:
		case VK_F8:
		case VK_F9:
		case VK_F10:
		case VK_F11:
		case VK_F12:
		case VK_F13:
		case VK_F14:
		case VK_F15:
		case VK_F16:
		case VK_F17:
		case VK_F18:
		case VK_F19:
		case VK_F20:
		case VK_F21:
		case VK_F22:
		case VK_F23:
		case VK_F24:
		case VK_LBUTTON:
		case VK_RBUTTON:
		case VK_LCONTROL:
		case VK_RCONTROL:
			break;
		// special effect, toggle the capslock
		case VK_CAPITAL:
			this->mpState->mInput.isCaps = !this->mpState->mInput.isCaps;
			break;
		// special effect, remove the last character in the stream
		case VK_BACK:
			{
				// get teh current line
				std::string str = this->mpState->mInput.currentLine;
				// ensure that the line has content to begin with
				if (str.length() > 0)
				{
					// find the substring without the last character, the current string buffer becomes that
					this->mpState->mInput.currentLine = str.substr(0, str.length() - 1);
				}
			}
			break;
		// special effect, push the text line into the records buffer
		// only if there is text in the buffer
		case VK_RETURN:
			if (this->mpState->mInput.allowEmptyLines || this->mpState->mInput.currentLine.length() > 0)
			{

				// Mark the latest line
				this->onInput(this->mpState->mInput.currentLine);

				// The line size should be cleared, as the new line's previous size is DNE
				this->mpState->mInput.lineSizePrevious = -1;

				// empty the string (size is now 0)
				this->mpState->mInput.currentLine = "";

			}
			break;
		default:
			{
				char character = (char)MapVirtualKey(i, MAPVK_VK_TO_CHAR);
				if (this->mpState->mInput.isCaps || current[VK_SHIFT] || current[VK_LSHIFT] || current[VK_RSHIFT])
				{
					if (i == '1')
					{
						character = '!';
					}
					else if (i == '2')
					{
						character = '@';
					}
					else if (i == '3')
					{
						character = '#';
					}
					else if (i == '4')
					{
						character = '$';
					}
					else if (i == '5')
					{
						character = '%';
					}
					else if (i == '6')
					{
						character = '^';
					}
					else if (i == '7')
					{
						character = '&';
					}
					else if (i == '8')
					{
						character = '*';
					}
					else if (i == '9')
					{
						character = '(';
					}
					else if (i == '0')
					{
						character = ')';
					}
				}
				else
				{
					character = tolower(character);
				}

				// Update the current line with the new character
				this->mpState->mInput.currentLine += character;
				
			}
			break;
	}

}

/** Author: Dustin Yost
* Called when some input has been entered (ENTER has been pressed)
*/
void StateServer::onInput(std::string &input)
{
	std::cout << input << '\n';
}

void StateServer::start()
{
	ChampNetPlugin::StartServer(this->mpState->mNetwork.port, this->mpState->mNetwork.maxClients);
}

void StateServer::disconnect()
{
	ChampNetPlugin::Disconnect();
}

void StateServer::render()
{

	// The current line being typed
	std::string input = this->mpState->mInput.currentLine;
	// The previous size of the line being typed
	// will be the same as input.length() when no input has changed
	// will be -1 if the line was inputted this update
	// will be input.length() - 1 if there was a letter added this update
	// will be input.length() + 1 if there was a letter removed this update
	unsigned int inputSizePrev = this->mpState->mInput.lineSizePrevious;

	// The current position of the cursor (the next letter will be written here)
	int cursorPosX = this->mpState->mConsole.cursorPosX;
	int cursorPosY = this->mpState->mConsole.cursorPosY;

}
