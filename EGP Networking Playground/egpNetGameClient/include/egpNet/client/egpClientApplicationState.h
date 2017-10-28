/*
	egpClientApplicationState.h
	By Dan Buckstein

	Interface for client application development.

	Additional contributions:
*/

#ifndef __EGP_CLIENTAPPLICATIONSTATE_H_
#define __EGP_CLIENTAPPLICATIONSTATE_H_


#include "egpNet/egpApplicationState.h"
#include "egpNet/client/egpWindowState.h"


class egpClientApplicationState : public egpApplicationState
{

	const egpWindowState *mpk_windowState;

public: 

	egpClientApplicationState();
	virtual ~egpClientApplicationState();


	// overloaded callbacks
	virtual int OnIdle();

	// set window state
	void SetWindowState(const egpWindowState *windowState);

};


#endif	// !__EGP_CLIENTAPPLICATIONSTATE_H_