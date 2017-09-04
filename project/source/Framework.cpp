/*
	Dustin Yost 0984932
	EGP-405-02
	Lab1
	09/03/2017
	We certify that this work is entirely our own.
	The assessor of this project may reproduce this project and provide
	copies to other academic staff, and/or communicate a copy of this project
	to a plagiarism-checking service, which may retain a copy of
	the project on its database.
*/
#include "Framework.h"

#include <iostream>

Framework::Framework() {

	this->consoleWindow = GetForegroundWindow();
	
	// Create the network and a packethandler for the network
	this->network = this->createNetwork();
	this->packetHandler = new NetworkImpl::PacketHandler(this->network->isServer);
	this->packetHandler->subscribeTo(this->network);

	// Initialize the socket connection
	this->network->startup();

	// Notify user that the game has started
	printf("Starting the %s.\n", this->network->isServer ? "server" : "client");

	// Setup the server or Connect from client to server
	this->network->connect();

}

Framework::~Framework() {
	// Destroy the connection
	delete this->network;
	delete this->packetHandler;
}

Network::Network* Framework::createNetwork() {

	// Prompt user for required information
	this->prompt();

	// Create and return the appropriate network object
	if (this->isServer) {
		return new Network::NetworkServer(this->port, this->maxClients);
	}
	else {
		return new Network::NetworkClient(this->address, this->port);
	}
}

void Framework::prompt() {
	char str[512];

	// Get whether to start as a client or server
	printf("(C) or (S)erver? ");
	fgets(str, 512, stdin);
	// true if the user input spcifies client
	bool isInputClient = (str[0] == 'c') || (str[0] == 'C');
	this->isServer = !isInputClient;

	printf("Server Port: ");
	std::cin >> this->port;
	fgets(str, 512, stdin); // clean up input

	this->maxClients = 0;
	this->address = "";
	if (this->isServer) {
		// Get max clients if creating a server
		printf("Max Clients: ");
		std::cin >> this->maxClients;
		fgets(str, 512, stdin); // clean up input
	}
	else {
		// Get the IP to connect to
		printf("Enter server IP or hit enter for 127.0.0.1... ");
		fgets(str, 512, stdin);
		if (str[0] == '\n') {
			strcpy(str, "127.0.0.1");
		}
		this->address = std::string(str);
	}

}

void Framework::run() {

	// Search for packet messages
	bool shouldExit = false;
	while (!shouldExit)
	{
		// Update the network
		this->network->update();
		// Check for user input
		this->input(shouldExit);
	}

}

void Framework::input(bool &shouldExit) {

	// Do not handle WINAPI input if this window is not active
	if (GetForegroundWindow() != this->consoleWindow) {
		return;
	}

	// check for escape key
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {

		// CLIENT-SIDE: Must notify the server that the client is leaving
		if (!this->network->isServer) {
			this->network->disconnect(
				NetworkImpl::PacketMessage{ (unsigned char)NetworkImpl::ID_CLIENT_LEFT, "Client is leaving." }
			);
		}

		shouldExit = true;
	}

}
