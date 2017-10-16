#ifndef CHAMPNET_SERVER_SERVER_H
#define CHAMPNET_SERVER_SERVER_H

#include "Game.h"

class Server : public Game
{

public:

	Server();
	virtual ~Server();

	void start();
	void disconnect();

};

#endif // CHAMPNET_SERVER_SERVER_H