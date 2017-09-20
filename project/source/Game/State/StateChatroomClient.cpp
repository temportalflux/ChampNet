#include "Game\State\StateChatroomClient.h"

#include "Network\Network.h"
#include "Game\Packets\Packet.h"
#include <sstream>

using std::stringstream;

// AUTHOR: Dustin Yost
void StateChatroomClient::doHandlePacket(Network::PacketInfo info) {
	switch (info.getPacketType()) {
		case ID_CONNECTION_REQUEST_ACCEPTED: // Handle connections
			// TODO: Send username response to server
			break;
		case ID_NEW_CLIENT_JOINED: // Handle the message saying some client has joined
			{
				stringstream msg;
				msg << "User " << ((PacketString*)info.data)->content << " has joined.";
				this->pushMessage(msg.str());
			}
			break;
		case ID_CLIENT_NUMBER: // Handle incoming assignment to the user id
			this->pushMessage("Welcome to the server");
			break;
		default:
			break;
	}
}

void StateChatroomClient::render() {

}
