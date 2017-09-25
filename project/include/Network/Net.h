#pragma once

#include <RakNet\RakNetTypes.h>
#include <string>

namespace RakNet {
	class Packet;
	class RakPeerInterface;
}

class MessageHandler;

namespace Network {

	class Net {

	protected:
		bool mIsServer;
		// The interface used to connect to another computer
		RakNet::RakPeerInterface *mpPeerInterface;
		// The port this connection goes through
		unsigned int port;
		MessageHandler *mpPacketHandler;

		// Abstract method to handle packets for clients and servers
		void handlePacket(RakNet::Packet *packet);

	public:

		// Base constructor, initializes the peer interface
		Net(MessageHandler* handler, bool isServer);
		Net(MessageHandler* handler) : Net(handler, false) {}
		virtual ~Net();

		void queryAddress(RakNet::SystemAddress &address);
		std::string getIP();

		// Handles updates for the network, specifically searching for new packets
		void update();

		void disconnect();

	};

}
