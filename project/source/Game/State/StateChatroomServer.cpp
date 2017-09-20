#include "Game\State\StateChatroomServer.h"

#include "Game\Packets\Packet.h"
#include "Network\Packets\PacketInfo.h"

void StateChatroomServer::doHandlePacket(Network::PacketInfo info) {
	switch (info.getPacketType()) {
		case ID_USERNAME: // Handle connections
			this->pushMessage("User joined.");
			break;
		default:
			break;
	}
}

void StateChatroomServer::render() {
	this->renderConsole();
}

void StateChatroomServer::sendPacket(Packet packet) {
	Network::PacketInfo i;
	i.address = NULL;
	i.network = NULL;
	i.data = (unsigned char*)(&packet);
	i.length = sizeof(packet);
	this->handlePacket(i);
}

