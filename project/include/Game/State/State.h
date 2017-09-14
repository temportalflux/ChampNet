#pragma once

class Game;

class State {

public:
	virtual ~State();

	virtual void onEnter(Game* game);
	virtual void update(Game* game);
	virtual void onExit(Game* game);

};
