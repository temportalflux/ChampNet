#include "ChampNetPlugin.h"

#include "Packet.h"
#include "ChampNet.h"

#include <string>
#include <sstream>
#include <iostream>

#include <RakNet\RakPeerInterface.h>
#include <RakNet\GetTime.h>

namespace ChampNetPlugin {

	CHAMPNET_PLUGIN_SYMTAG ChampNet::Network *gpNetwork = 0;
	static FuncCallBack gDebugFunc = 0;

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
		if (gDebugFunc != 0)
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
			send_log("Starting the client", Color::Green);
			gpNetwork->initClient();
			return 0;
		}

		return 1;
	}

	// Connect this CLIENT to some server using the specified credentials
	int ConnectToServer(const char* address, int port)
	{
		if (gpNetwork != 0)
		{
			std::stringstream s("Connectint to ");
			s << address << '|' << port;
			send_log(s.str().c_str(), Color::Green);

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
		if (validPacket)
		{
			std::stringstream s;
			s << "Found valid packet at location " << (void const*)packet;
			send_log(s.str().c_str(), Color::Green);
		}

		// Send the packet ptr for usage by FreePacket
		return (void *)packet;
	}

	// Returns the packet's address, given some valid packet pointer (Call after PollPacket if valid is true).
	const char* GetPacketAddress(void* packetPtr, unsigned int &length)
	{
		length = 0;

		ChampNet::Packet *packet = (ChampNet::Packet*)packetPtr;
		bool valid = packet != NULL;

		char *out = NULL;
		if (valid)
		{
			const char *address;
			packet->getAddress(address, length);

			out = new char[length + 1];
			for (unsigned int i = 0; i < length; i++)
			{
				out[i] = address[i];
			}
			out[length] = '\0';
		}
		else
		{
			out = new char[1];
			out[length] = '\0';
		}

		return out;
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

	void SendByteArray(const char* address, int port, char* byteArray, int byteArraySize)
	{
		//gDebugFunc(address, (int)ChampNetPlugin::Color::White, std::strlen(address));
		//gDebugFunc(byteArray, (int)ChampNetPlugin::Color::White, byteArraySize);
		std::stringstream s;
		s << address << '|' << port;
		PacketPriority priority = HIGH_PRIORITY;
		PacketReliability reliability = RELIABLE;

		const RakNet::Time packetTime_local = RakNet::GetTime(); // Added By Jake
		const int sizeOfTimeStamp = sizeof(char) + sizeof(RakNet::Time) + sizeof(RakNet::Time); // Added By Jake
		const int totalSize = byteArraySize + sizeOfTimeStamp + sizeOfTimeStamp; // Added By Jake

		char* msg = new char[totalSize]; // Added By Jake
		char *msgPtrTmp = msg + WriteTimeStamp(msg, packetTime_local, packetTime_local); // Added By Jake
		char *msgPtr = msgPtrTmp + WriteTimeStamp(msgPtrTmp, 0, 0); // Added By Jake
		
		*msgPtrTmp = (char)byteArray[0]; // Added By Jake

		memcpy(msgPtr, byteArray, byteArraySize); // Added By Jake

		SendData(s.str().c_str(), msg, totalSize, &priority, &reliability, 0, false);
	}


	// Write the time stamp to a buffer
	int WriteTimeStamp(char *buffer, const RakNet::Time &t, const RakNet::Time &t0)
	{
		const unsigned int ret = (sizeof(char) + sizeof(RakNet::Time) + sizeof(RakNet::Time));
		if (buffer)
		{
			*(buffer++) = (char)(ID_TIMESTAMP);
			RakNet::Time *tPtr = (RakNet::Time *)buffer;
			*(tPtr++) = t;
			*(tPtr++) = t0;
			return ret;
		}
		return 0;
	}

	// read the time stamp from a buffer
	int ReadTimeStamp(const char *buffer, RakNet::Time &t, RakNet::Time &t0)
	{
		const unsigned int ret = (sizeof(char) + sizeof(RakNet::Time) + sizeof(RakNet::Time));
		char tag;
		if (buffer)
		{
			tag = *(buffer++);
			const RakNet::Time *tPtr = (RakNet::Time *)buffer;
			t = *(tPtr++);
			t0 = *(tPtr++);
			if (*(buffer + 4) < 0)
				t += 4311744512;	// RakNet seems to be subtracting this number for some stupid reason... and only half the time... what is it doing
			return ret;
		}
		return 0;
	}

	void SendData(const char* address, char* byteArray, int byteArraySize, PacketPriority *priority, PacketReliability *reliability, int channel, bool broadcast)
	{
		RakNet::SystemAddress system_address;
		system_address.FromString(address);
		gpNetwork->sendTo(byteArray, byteArraySize, &system_address, priority, reliability, channel, broadcast);
	}

	void Disconnect()
	{
		gpNetwork->disconnect();
	}

	// Returns the current peer interface address
	const char* GetAddress()
	{
		return gpNetwork->getIP().c_str();
	}

}
