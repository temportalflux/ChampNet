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

	enum PacketPriority;
	enum PacketReliability;

	// Author: Dustin Yost
	namespace ChampNetPlugin {

		enum MessageIDs
		{
			// RakNet messages (unsued for clients)
			ID_CLIENT_CONNECTION = ID_NEW_INCOMING_CONNECTION,
			// The connection was severed
			ID_CLIENT_DISCONNECTION = ID_DISCONNECTION_NOTIFICATION,
			// The connection was dropped
			ID_CLIENT_MISSING = ID_CONNECTION_LOST,

			// RakNet Messages (used for clients)
			ID_CLIENT_CONNECTION_ACCEPTED = ID_CONNECTION_REQUEST_ACCEPTED,
			ID_CLIENT_CONNECTION_REJECTED = ID_CONNECTION_ATTEMPT_FAILED,

			// placeholder - For tracking in C# script
			ID_PLACEHOLDER = ID_USER_PACKET_ENUM,

			// Client-Sent Messages
			// 1) Sent to server to notify it of an incoming client
			ID_CLIENT_JOINED,
			// Sent to server to notify all clients of an updated position
			ID_USER_UPDATE_POSITION,
			// Sent to server to request a battle with some other player
			ID_BATTLE_REQUEST,
			// Sent to server to accept or deny battle with some requesting player
			ID_BATTLE_RESPONSE,

			// Server-Sent Messages
			// 2) Sent to clients to notify them of the values for some spawning user
			// Sender uses to place self, peers use to place a dummy unit
			ID_USER_SPAWN,
			// 3) Sent to clients to mandate their ID
			ID_USER_ID,
			// Send to all clients to notify them of a battle result
			ID_BATTLE_RESULT,
			// Sent to server and forwarded to clients notifying them a user has left the server
			ID_CLIENT_LEFT,
			// Notification to clients that the server has been disconnected
			ID_DISCONNECT,

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

		CHAMPNET_PLUGIN_SYMTAG void SendByteArray(const char* address, int port, char* byteArray, int byteArraySize);

		CHAMPNET_PLUGIN_SYMTAG void SendData(const char* address, char* byteArray, int byteArraySize, PacketPriority *priority, PacketReliability *reliability, int channel, bool broadcast);

		// Disconnect from the interface
		CHAMPNET_PLUGIN_SYMTAG void Disconnect();

		// Returns the current peer interface address
		CHAMPNET_PLUGIN_SYMTAG const char* GetAddress();

	};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _CHAMPNET_PLUGIN_PLUGIN_H