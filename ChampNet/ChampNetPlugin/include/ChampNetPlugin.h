#ifndef _CHAMPNET_PLUGIN_PLUGIN_H
#define _CHAMPNET_PLUGIN_PLUGIN_H

// defines CHAMPNET_PLUGIN_SYMTAG
#include "lib.h"

// Include raknet message identifiers
#include <RakNet\MessageIdentifiers.h>

// tell compiler to link as if all function are C not C++
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	namespace ChampNetPlugin {

		enum MessageIDs
		{
			// RakNet messages (unsued for clients)
			ID_CLIENT_CONNECTION = ID_NEW_INCOMING_CONNECTION,
			ID_CLIENT_DISCONNECTION = ID_DISCONNECTION_NOTIFICATION,
			ID_CLIENT_MISSING = ID_CONNECTION_LOST,

			// placeholder - For tracking in C# script
			ID_PLACEHOLDER = ID_USER_PACKET_ENUM,

			// Server-Sent Messages
			// 2) Sent to clients to notify them of the values for some spawning user
			// Sender uses to place self, peers use to place a dummy unit
			ID_USER_SPAWN,

			// Client-Sent Messages
			// 1) Sent to server to notify it of an incoming client
			ID_USER_JOINED,

		};

		// Create a network to connect with
		CHAMPNET_PLUGIN_SYMTAG int Create();

		typedef void(*FuncCallBack)(const char* message, int color, int size);
		enum class Color { Red, Green, Blue, Black, White, Yellow, Orange };

		CHAMPNET_PLUGIN_SYMTAG void RegisterDebugCallback(FuncCallBack callBack);

		static void send_log(const char *msg, const Color &color);

		// Destroy the network (must call Create prior) (must call when owning object is destroyed)
		CHAMPNET_PLUGIN_SYMTAG int Destroy();

		// Start a server with the specified credentials using this object
		CHAMPNET_PLUGIN_SYMTAG int StartServer(int port, int maxClients);

		// Start a client using this object
		CHAMPNET_PLUGIN_SYMTAG int StartClient();

		// Connect this CLIENT to some server using the specified credentials
		CHAMPNET_PLUGIN_SYMTAG int ConnectToServer(const char* address, int port);

		// Fetch all incoming packets. Must call prior to PollPacket. Must be called after Create and before Destroy. Returns the quantity of packets in the queue after fetch.
		CHAMPNET_PLUGIN_SYMTAG int FetchPackets();

		// Poll the packet queue. Returns a pointer to the first packet, and removes that packet from the queue. If valid is true, then the packet can be processed, else the packet does not exist (no packets presently in the queue).
		CHAMPNET_PLUGIN_SYMTAG void* PollPacket(bool &validPacket);

		// Returns the packet's address, given some valid packet pointer (Call after PollPacket if valid is true).
		CHAMPNET_PLUGIN_SYMTAG const char* GetPacketAddress(void* packetPtr, unsigned int &length);

		// Returns the packet's data, given some valid packet pointer (Call after PollPacket if valid is true).
		CHAMPNET_PLUGIN_SYMTAG unsigned char* GetPacketData(void* packetPtr, unsigned int &length);

		// Frees the memory of some packet, given some valid packet pointer (Call after PollPacket if valid is true).
		CHAMPNET_PLUGIN_SYMTAG void FreePacket(void* packetPtr);

		// Disconnect from the interface
		CHAMPNET_PLUGIN_SYMTAG void Disconnect();

	};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _CHAMPNET_PLUGIN_PLUGIN_H