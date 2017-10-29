/*
	egpServerApplicationState.cpp
	By Dan Buckstein

	Implementation for server application development.

	Additional contributions:
*/

#include "egpNet/server/egpServerApplicationState.h"

#include "ChampNet\ChampNet.h"

egpServerApplicationState::egpServerApplicationState()
	: egpApplicationState()
{
	mpNetwork->initServer(425, 1);
}

egpServerApplicationState::~egpServerApplicationState()
{
	mpNetwork->disconnect();
}


// callbacks
int egpServerApplicationState::OnIdle()
{
	// ****TO-DO: SERVER IDLE: 
	//	-> process inputs
	//		-> devices
	//		-> remote state(s)
	//		-> local state
	//	-> parent update where appropriate

	// parent update
	egpApplicationState::OnIdle();

	// nothing happened
	return 0;
}
