#include "Game\State\StateGameNetwork.h"

#include "Network\PacketInfo.h"
#include <iostream>

StateGameNetwork::StateGameNetwork(Net *network) : StateGame(), mpNetwork(network) {
}

StateGameNetwork::~StateGameNetwork() {
	if (mpNetwork != NULL) {
		delete mpNetwork;
		mpNetwork = NULL;
	}
}

void StateGameNetwork::updateNetwork() {

}

void StateGameNetwork::handlePacket(PacketInfo *info) {
	switch (info->getPacketType()) {

		default:
			break;
	}
}
