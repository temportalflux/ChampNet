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

