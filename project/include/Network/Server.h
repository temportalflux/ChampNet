/*
Dustin Yost 0984932
Jon Trusheim 0979380
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

#include "Framework.h"

// Author: Dustin Yost
class Server : public Framework {

protected:
	unsigned int mMaxClients;

	virtual Network::Network* createNetwork();

public:
	Server(Network::PacketHandler* packetHandler, FrameworkData data);
	virtual ~Server();

};
