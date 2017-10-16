#ifndef CHAMPNET_SERVER_SERVER_H
#define CHAMPNET_SERVER_SERVER_H

#include "Game.h"

class Server : public Game
{

public:

	Server();
	virtual ~Server();

	virtual void onKeyDown(int i) override;
	virtual void onInput(std::string &input);
	
	void start();
	void disconnect();

	virtual void render() override;

};

#endif // CHAMPNET_SERVER_SERVER_H