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

#include <string>

struct FrameworkData {
	// The port of the server (this server if isServer is true, else the server this client is connecting to)
	unsigned int port;
	// SERVER ONLY: the max number of clients for the server
	unsigned int maxClients;
	// CLIENT ONLY: the address of the server to connect to
	char serverAddress[15];// ***.***.***.*** 12+3=15
	// CLIENT ONLY: the username of the user
	std::string username;
};

