#ifndef _CHAMPNET_PLUGIN_PLUGIN_H
#define _CHAMPNET_PLUGIN_PLUGIN_H

// defines CHAMPNET_PLUGIN_SYMTAG
#include "lib.h"

// tell compiler to link as if all function are C not C++
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	namespace ChampNetPlugin {

		// Create a network to connect with
		CHAMPNET_PLUGIN_SYMTAG int Create();

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
		CHAMPNET_PLUGIN_SYMTAG char* GetPacketAddress(void* packetPtr, unsigned int &length);

		// Returns the packet's data, given some valid packet pointer (Call after PollPacket if valid is true).
		CHAMPNET_PLUGIN_SYMTAG unsigned char* GetPacketData(void* packetPtr, unsigned int &length);

		// Frees the memory of some packet, given some valid packet pointer (Call after PollPacket if valid is true).
		CHAMPNET_PLUGIN_SYMTAG void FreePacket(void* packetPtr);

	};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _CHAMPNET_PLUGIN_PLUGIN_H