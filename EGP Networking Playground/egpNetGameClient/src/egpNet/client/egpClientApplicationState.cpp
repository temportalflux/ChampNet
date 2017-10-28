/*
	egpClientApplicationState.cpp
	By Dan Buckstein

	Implementation for client application development.

	Additional contributions:
*/

#include "egpNet/client/egpClientApplicationState.h"

#include "ChampNet\ChampNet.h"
#include "egpNet\utils\egpInput.h"
#include <stdio.h>

egpClientApplicationState::egpClientApplicationState()
	: egpApplicationState()
	, mpk_windowState(0)
{
	mpNetwork->initClient();
}

egpClientApplicationState::~egpClientApplicationState()
{
	if (mpNetwork->isActive())
		mpNetwork->disconnect();
}


// callbacks
int egpClientApplicationState::OnIdle()
{
	// ****TO-DO: CLIENT IDLE: 
	//	-> process inputs
	//		-> devices
	//		-> remote state
	//		-> local state
	//	-> rendering (client probably has a render window, server probably does not)
	//	-> parent update where appropriate

	// parent update
	egpApplicationState::OnIdle();

	printf("%d\n", egpKeyboardKeyWasPressed(this->m_keyboard, 'C'));
	if (egpKeyboardKeyWasPressed(this->m_keyboard, 'c'))
	{
		if (this->mpNetwork->isActive())
		{
			printf("Already connected, cannot connect to server\n");
		}
		else
		{
			printf("Connect to server pls...\n");
			this->mpNetwork->connectToServer("127.0.0.1", 425);
		}
	}
	if (egpKeyboardKeyWasPressed(this->m_keyboard, 'D'))
	{
		if (this->mpNetwork->isActive())
		{
			printf("Disconnect to server pls...\n");
			this->mpNetwork->disconnect();
		}
		else
		{
			printf("Not connected to server, cannot disconnect\n");
		}
	}

	// nothing happened
	return 0;
}

// setters
void egpClientApplicationState::SetWindowState(const egpWindowState *windowState)
{
	if (windowState->userData == this)
		mpk_windowState = windowState;
}
