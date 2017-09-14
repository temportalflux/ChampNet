#pragma once

class State;
class Game;

class StateMachine {

private:
	State* mpState;
	State* mpStateNext;

public:
	StateMachine();
	~StateMachine();

	void queueState(State* nextState);
	void update(Game* game);
	void cleanState();

};
