#pragma once

class State;
class Game;

enum EnumState {
	NONE,
	LOGIN,
};

class StateMachine {

private:
	State* mpState;
	EnumState mStateCurrent;
	EnumState mStateNext;

	State* createStateFor(EnumState state);

public:
	StateMachine();
	~StateMachine();

	void queueState(EnumState state);
	void update(Game* game);
	void cleanState();

};
