#include "Game\State\StateGameNetwork.h"

#include "Network\PacketInfo.h"
#include <iostream>

void StateGameNetwork::updateNetwork() {

}

void StateGameNetwork::handlePacket(PacketInfo *info) {
	switch (info->getPacketType()) {

		default:
			break;
	}
}
