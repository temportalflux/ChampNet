/*
	egpApplicationState.cpp
	By Dan Buckstein

	Base implementation for networked application development.

	Additional contributions: 
*/

#include "egpNet/egpApplicationState.h"

#include "ChampNet\ChampNet.h"

egpApplicationState::egpApplicationState()
{
	// reset inputs
	egpKeyboardReset(m_keyboard);
	egpMouseReset(m_mouse);
	mpNetwork = new ChampNet::Network();
}

egpApplicationState::~egpApplicationState()
{
	if (mpNetwork != NULL)
	{
		delete mpNetwork;
		mpNetwork = NULL;
	}
}


// common callbacks
int egpApplicationState::OnIdle()
{
	// example idle: update inputs
	egpKeyboardUpdate(m_keyboard);
	egpMouseUpdate(m_mouse);

	// done, indicate that nothing important happened
	return 0;
}

int egpApplicationState::OnKeyPress(unsigned char key)
{
	// press key
	m_keyboard->state.key[key] = 1;

	// done
	return 1;
}

int egpApplicationState::OnKeyRelease(unsigned char key)
{
	// release key
	m_keyboard->state.key[key] = 0;

	// done
	return 0;
}

int egpApplicationState::OnMouseButtonPress(unsigned char button, int cursorX, int cursorY)
{
	// press button
	m_mouse->state.btn[button] = 1;
	
	// set position
	m_mouse->state.x = cursorX;
	m_mouse->state.y = cursorY;

	// done
	return 1;
}

int egpApplicationState::OnMouseButtonRelease(unsigned char button, int cursorX, int cursorY)
{
	// press button
	m_mouse->state.btn[button] = 0;

	// set position
	m_mouse->state.x = cursorX;
	m_mouse->state.y = cursorY;

	// done
	return 0;
}

int egpApplicationState::OnMouseWheel(int delta, int cursorX, int cursorY)
{
	// change mouse wheel
	m_mouse->state.wheel += delta;

	// set position
	m_mouse->state.x = cursorX;
	m_mouse->state.y = cursorY;

	// done
	return delta;
}

int egpApplicationState::OnMouseMove(int cursorX, int cursorY)
{
	// set position
	m_mouse->state.x = cursorX;
	m_mouse->state.y = cursorY;

	// done
	return 0;
}
