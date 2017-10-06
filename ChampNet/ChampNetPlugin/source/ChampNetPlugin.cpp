#include "ChampNetPlugin.h"

#include "Packet.h"
#include "ChampNet.h"

#include <string>

namespace ChampNetPlugin {

	CHAMPNET_PLUGIN_SYMTAG ChampNet::Network *gpNetwork = 0;

	int Create() {
		if (gpNetwork == 0)
		{
			gpNetwork = new ChampNet::Network();
			return true;
		}
		return 0;
	}

	int Destroy() {
		if (gpNetwork != 0)
		{
			if (gpNetwork->isActive())
			{
				gpNetwork->disconnect();
			}
			delete gpNetwork;
			gpNetwork = 0;
		}
		return 1;
	}

	// Startup the server interface
	int StartServer(int port, int maxClients) {
		if (gpNetwork != 0)
		{
			gpNetwork->initServer(port, maxClients);
			return 0;
		}
		return 1;
	}

	// Startup the client interface
	int StartClient()
	{
		if (gpNetwork != 0)
		{
			gpNetwork->initClient();
			return 0;
		}
		return 1;
	}

	// Connect the interface to its destination
	int ConnectToServer(std::string &address, int port)
	{
		if (gpNetwork != 0)
		{
			gpNetwork->connectToServer(address, port);
			return 0;
		}
		return 1;
	}

	void FetchPackets()
	{
		gpNetwork->fetchAllPackets();
	}

	// TODO: Pass serialized data (all entries of gpPackets) back to caller
	bool PollPacket()
	{
		ChampNet::Packet *packet = NULL;
		bool validPacket = gpNetwork->pollPackets(packet);

		delete packet;

		return validPacket;
	}

}
