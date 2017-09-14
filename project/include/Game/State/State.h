#pragma once

class Game;
struct StateInput;
struct StateNetwork;
struct StateGame;

class State {

public:
	virtual ~State();

	virtual void onEnter(Game* game);
	virtual void update(Game* game);
	virtual void handleInput(StateInput* state);
	virtual void handleNetwork(StateNetwork* state);
	virtual void handleGameUpdates(StateGame* state, Game* game);
	virtual void onExit(Game* game);

};
