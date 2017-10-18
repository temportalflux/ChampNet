#include "ChampNetPlugin.h"

#include "Packet.h"
#include "ChampNet.h"

#include <string>
#include <sstream>

namespace ChampNetPlugin {

	CHAMPNET_PLUGIN_SYMTAG ChampNet::Network *gpNetwork = 0;
	static FuncCallBack gDebugFunc = nullptr;

	// Create a network to connect with
	int Create() {
		if (gpNetwork == 0)
		{
			gpNetwork = new ChampNet::Network();
			return true;
		}
		return 0;
	}

	void RegisterDebugCallback(FuncCallBack cb)
	{
		gDebugFunc = cb;
		send_log("Initialized debug callback", Color::Black);
	}

	void send_log(const char *msg, const Color &color)
	{
		if (gDebugFunc != nullptr)
			gDebugFunc(msg, (int)color, (int)strlen(msg));
	}

	// Destroy the network (must call Create prior) (must call when owning object is destroyed)
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

	// Start a server with the specified credentials using this object
	int StartServer(int port, int maxClients) {
		if (gpNetwork != 0)
		{
			gpNetwork->initServer(port, maxClients);
			return 0;
		}
		return 1;
	}

	// Start a client using this object
	int StartClient()
	{
		if (gpNetwork != 0)
		{
			gpNetwork->initClient();
			return 0;
		}

		send_log("test", Color::Blue);
		return 1;
	}

	// Connect this CLIENT to some server using the specified credentials
	int ConnectToServer(const char* address, int port)
	{
		if (gpNetwork != 0)
		{
			gpNetwork->connectToServer(address, port);
			return 0;
		}
		return 1;
	}

	// Fetch all incoming packets. Must call prior to PollPacket. Must be called after Create and before Destroy. Returns the quantity of packets in the queue after fetch.
	int FetchPackets()
	{
		gpNetwork->fetchAllPackets();
		return gpNetwork->getPacketCount();
	}

	// Poll the packet queue. Returns a pointer to the first packet, and removes that packet from the queue. If valid is true, then the packet can be processed, else the packet does not exist (no packets presently in the queue).
	// Pass serialized data (all entries of gpPackets) back to caller
	// Pass by reference (&) is indicated by 'ref' in C#
	// Address is a string (const char*) passed by reference (&), and will appear in C# as 'ref string address'
	// 'unsigned int &' will appear in C# as 'ref uint'
	// 'unsigned char* &' will appear in C# as 'ref IntPtr'
	// Returns true if a valid packet was found, false if there are no packets in the buffer
	void* PollPacket(bool &validPacket)
	{
		ChampNet::Packet *packet = NULL;
		validPacket = gpNetwork->pollPackets(packet);

		// Send the packet ptr for usage by FreePacket
		return packet;
	}

	// Returns the packet's address, given some valid packet pointer (Call after PollPacket if valid is true).
	const char* GetPacketAddress(void* packetPtr, unsigned int &length)
	{
		// copies the REFERENCE (not the actual bytes)
		// this means data MUST be copied by the caller
		const char* address;
		((ChampNet::Packet*)packetPtr)->getAddress(address, length);
		return address;
	}

	// Returns the packet's data, given some valid packet pointer (Call after PollPacket if valid is true).
	unsigned char* GetPacketData(void* packetPtr, unsigned int &length)
	{
		// copies the REFERENCE (not the actual bytes)
		// this means data MUST be copied by the caller
		unsigned char* data;
		((ChampNet::Packet*)packetPtr)->getData(data, length);
		return data;
	}

	// Frees the memory of some packet, given some valid packet pointer (Call after PollPacket if valid is true).
	// Free some packet delegated from PollPacket
	void FreePacket(void* packetPtr)
	{
		delete (ChampNet::Packet*)packetPtr;
	}

	void Disconnect()
	{
		gpNetwork->disconnect();
	}

}
