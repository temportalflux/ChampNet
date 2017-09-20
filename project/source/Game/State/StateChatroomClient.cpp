#include "Game\State\StateChatroomClient.h"

#include "Network\Network.h"

// AUTHOR: Dustin Yost
void StateChatroomClient::doHandlePacket(Network::PacketInfo info) {
	switch (info.getPacketType()) {
		case ID_CONNECTION_REQUEST_ACCEPTED: // Handle connections

			break;
		default:
			break;
	}
}

void StateChatroomClient::render() {

}
