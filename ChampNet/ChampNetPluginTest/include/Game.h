#ifndef CHAMPNET_SERVER_GAME_H
#define CHAMPNET_SERVER_GAME_H

class Game
{

private:

	bool mIsRunning;

protected:

	virtual void updateInput();
	virtual void updateNetwork();
	virtual void updateGame();
	virtual void render();

public:

	Game();
	virtual ~Game();

	inline const bool isRunning() const { return mIsRunning; }

	void update();

};

#endif // CHAMPNET_SERVER_GAME_H