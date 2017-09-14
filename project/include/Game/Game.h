/*
	Dustin Yost 0984932
	EGP-405-02
	Project 1
	09/21/2017
	We certify that this work is entirely our own.
	The assessor of this project may reproduce this project and provide
	copies to other academic staff, and/or communicate a copy of this project
	to a plagiarism-checking service, which may retain a copy of
	the project on its database.
*/
#pragma once

#include "Game\State\GameState.h"

class Framework;
struct FrameworkData;
class InputSystem;
class StateMachine;

// Author: Dustin Yost
class Game {

private:
	Framework* mpNetworkFramework;
	InputSystem* mpSystemInput;
	StateMachine* mpStateMachine;
	StateGame mpStateGame[1];

public:
	Game();
	~Game();

	void run();
	void setNetworkType(bool isClient, FrameworkData data);
	StateGame* getGameState();

protected:

	void startup();
	void runLoop();

	void queueState(StateGame::EnumState nextState);
	void update();
	void processStateQueue();
	void updateState();

};
