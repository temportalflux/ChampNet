// INTRO TO NETWORKING ARCHITECTURE - ASYNC CHAT ROOM APPLICATION
#include <Windows.h>
#include "RakNet\RakPeerInterface.h"


struct KeyboardState
{
	unsigned char keyboard[256];
};


struct ApplicationState
{
	int running;

	KeyboardState key [1];
};


void updateInput(KeyboardState *key)
{
	// - get mouse, keyboard, controller state
	// - track this for later
	// example: in a char room...

	GetKeyboardState(key->keyboard);
}

void updateNetworking()
{
	// TMP: make some networking state
	// that stores a peer instance
	// for now a temp
	RakNet::RakPeerInterface *peer;


	// - receive packets until no more packets
	// - while packet exists
	// - process message
	// - track data (if any) for later
	RakNet::Packet *packet;
	for (packet - peer->Receive(); packet; peer->DeallocatePacket(packet), peer->Receive())
	{
		// HOORAY NETWORKING MESSAGES
	}



}

void updateState(ApplicationState *state)
{
	// - account for all previous updates here
	// - change appliclation with time & other updates

	// e.g. char room
	// - update chat stream
}

void render(const ApplicationState *state)
{
	// - "clear buffer"
	// - display state to display medium

	//e.g. chat room
	// - display chat stream (still rending);
}

int main()
{
	ApplicationState theState[1];
	theState->running = 1;
	while (theState->running)
	{
		updateInput(theState->key);

		updateNetworking(/**/);

		updateState(theState);


		render(theState);
	}
}