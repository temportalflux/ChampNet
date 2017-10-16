#include "Server.h"

#include "ChampNetPlugin.h"

Server::Server() : Game()
{
	ChampNetPlugin::Create();
}

Server::~Server()
{
	ChampNetPlugin::Destroy();
}

void Server::start()
{
	//ChampNetPlugin::StartServer(425, 0);
}

void Server::disconnect()
{
	ChampNetPlugin::Disconnect();
}
