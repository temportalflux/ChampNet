#include "Game\State\StateChatroomClient.h"

#include "Game\Packets\Packet.h"
#include <sstream>
#include "Network\Framework.h"

using std::stringstream;

// AUTHOR: Dustin Yost
void StateChatroomClient::doHandlePacket(Network::PacketInfo *info) {
	unsigned char id = info->getPacketType();
	switch (id) {
		case ID_CONNECTION_REQUEST_ACCEPTED: // Handle connections
			this->mAddressServer = info->address;
			{
				this->pushMessage("Connected.");

				PacketString packet;
				packet.packetID = GameMessages::ID_USERNAME;
				std::string username = this->mData.network->networkInfo.username;
				strncpy(packet.content, username.c_str(), username.length());

				this->sendPacket(packet);
			}
			break;
		case ID_NEW_CLIENT_JOINED: // Handle the message saying some client has joined
			{
				stringstream msg;
				msg << "User " << ((PacketString*)info->data)->content << " has joined.";
				this->pushMessage(msg.str());
			}
			break;
		case ID_CLIENT_NUMBER: // Handle incoming assignment to the user id
			this->pushMessage("Welcome to the server");
			break;
		default:
			this->pushMessage("Recieved message id " + id);
			break;
	}
}

void StateChatroomClient::render() {
	this->renderConsole();
}

void StateChatroomClient::sendPacket(Packet packet) {
	this->getFramework()->sendPacket(packet, this->mAddressServer);
}
