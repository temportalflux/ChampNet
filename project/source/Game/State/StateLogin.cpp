#include "Game\State\StateLogin.h"
#include <stdio.h>
#include "Game\Game.h"
#include "Network\Framework.h"

#include <iostream>

StateLogin::~StateLogin()
{

}

void StateLogin::handleInput(StateInput* state) {
	for (int key = 0; key < StateInput::SIZE_KEYBOARD; key++) {
		if (state->keyboard[key] & 0x8000) {
			std::cout << "Key " << key << " is down\n";
		}
	}
}

void StateLogin::handleGameUpdates(StateGame* state, Game* game) {

}

/*
void StateLogin::onEnter(Game* game)
{
	char str[512];

	// Get whether to start as a client or server
	printf("(C) or (S)erver? ");
	fgets(str, 512, stdin);
	// true if the user input spcifies client
	bool isClient = (str[0] == 'c') || (str[0] == 'C');

	FrameworkData data;

	printf("Server Port: ");
	std::cin >> data.port;
	fgets(str, 15, stdin); // clean up input

	if (isClient) {
		// Get the IP to connect to
		printf("Enter server IP or hit enter for 127.0.0.1... ");
		fgets(data.serverAddress, 15, stdin);
		if (data.serverAddress[0] == '\n') {
			strcpy(data.serverAddress, "127.0.0.1");
		}
	}
	else {
		// Get max clients if creating a server
		printf("Max Clients: ");
		std::cin >> data.maxClients;
		fgets(str, 15, stdin); // clean up input
	}

	game->setNetworkType(isClient, data);
}
//*/
