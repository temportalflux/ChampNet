/*
	egpApplicationState.h
	By Dan Buckstein

	Base interface for networked application development.
	NOTE: THIS IS NOT A GAME STATE; IT IS HIGHER LEVEL.

	Additional contributions: 
*/

#ifndef __EGP_APPLICATIONSTATE_H_
#define __EGP_APPLICATIONSTATE_H_


#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"

#include "utils/egpInput.h"

#include "egpGameState.h"

namespace ChampNet
{
	class Network;
};

class egpApplicationState abstract
{
protected: 

	egpApplicationState();
	virtual ~egpApplicationState();

	// common input device tracking
	egpKeyboard m_keyboard[1];
	egpMouse m_mouse[1];

	ChampNet::Network *mpNetwork;

public: 

	// common callbacks
	virtual int OnIdle();
	virtual int OnKeyPress(unsigned char key);
	virtual int OnKeyRelease(unsigned char key);
	virtual int OnMouseButtonPress(unsigned char button, int cursorX, int cursorY);
	virtual int OnMouseButtonRelease(unsigned char button, int cursorX, int cursorY);
	virtual int OnMouseWheel(int delta, int cursorX, int cursorY);
	virtual int OnMouseMove(int cursorX, int cursorY);

};


#endif	// !__EGP_APPLICATIONSTATE_H_