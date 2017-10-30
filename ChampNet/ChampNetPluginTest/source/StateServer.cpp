#include "StateServer.h"

#include "ChampNetPlugin.h"
#include "Packet.h"

#include "StateData.h"
#include "Win.h"
#include <iostream>
#include <RakNet\PacketPriority.h>
#include "Packets.h"

#include <stdlib.h> // rand

StateServer::StateServer() : StateApplication()
{
	ChampNetPlugin::Create();
}

StateServer::~StateServer()
{
	if (this->mpPlayerAddresses != NULL)
	{
		for (int i = 0; i < mPlayerAddressesLength; i++)
			if (mpPlayerAddresses[i] != NULL)
			{
				delete mpPlayerAddresses[i];
				mpPlayerAddresses[i] = NULL;
			}
		delete[] this->mpPlayerAddresses;
		this->mpPlayerAddresses = NULL;
	}

	this->disconnect();
	ChampNetPlugin::Destroy();
}

void StateServer::onEnterFrom(StateApplication *previous)
{
	StateApplication::onEnterFrom(previous);
	this->start();
}

/** Author: Dustin Yost
* Called when a key is marked as down this update
*/
void StateServer::onKeyDown(int i)
{
	StateApplication::onKeyDown(i);
	char *current = this->mpState->mInput.keyboard;

	// Check to see if the user's input has stopped the game
	if (!this->isRunning())
	{
		this->disconnect();
		return;
	}

	// Update the line size to track the previous size
	this->mpState->mInput.lineSizePrevious = (unsigned int)this->mpState->mInput.currentLine.length();

	switch (i)
	{
		// These should not have an input effect
		case VK_SHIFT:
		case VK_LSHIFT:
		case VK_RSHIFT:
		case VK_CONTROL:
		case VK_F1:
		case VK_F2:
		case VK_F3:
		case VK_F4:
		case VK_F5:
		case VK_F6:
		case VK_F7:
		case VK_F8:
		case VK_F9:
		case VK_F10:
		case VK_F11:
		case VK_F12:
		case VK_F13:
		case VK_F14:
		case VK_F15:
		case VK_F16:
		case VK_F17:
		case VK_F18:
		case VK_F19:
		case VK_F20:
		case VK_F21:
		case VK_F22:
		case VK_F23:
		case VK_F24:
		case VK_LBUTTON:
		case VK_RBUTTON:
		case VK_LCONTROL:
		case VK_RCONTROL:
			break;
		// special effect, toggle the capslock
		case VK_CAPITAL:
			this->mpState->mInput.isCaps = !this->mpState->mInput.isCaps;
			break;
		// special effect, remove the last character in the stream
		case VK_BACK:
			{
				// get teh current line
				std::string str = this->mpState->mInput.currentLine;
				// ensure that the line has content to begin with
				if (str.length() > 0)
				{
					// find the substring without the last character, the current string buffer becomes that
					this->mpState->mInput.currentLine = str.substr(0, str.length() - 1);
				}
			}
			break;
		// special effect, push the text line into the records buffer
		// only if there is text in the buffer
		case VK_RETURN:
			if (this->mpState->mInput.allowEmptyLines || this->mpState->mInput.currentLine.length() > 0)
			{

				// Mark the latest line
				this->onInput(this->mpState->mInput.currentLine);

				// The line size should be cleared, as the new line's previous size is DNE
				this->mpState->mInput.lineSizePrevious = -1;

				// empty the string (size is now 0)
				this->mpState->mInput.currentLine = "";

			}
			break;
		default:
			{
				char character = (char)MapVirtualKey(i, MAPVK_VK_TO_CHAR);
				if (this->mpState->mInput.isCaps || current[VK_SHIFT] || current[VK_LSHIFT] || current[VK_RSHIFT])
				{
					if (i == '1')
					{
						character = '!';
					}
					else if (i == '2')
					{
						character = '@';
					}
					else if (i == '3')
					{
						character = '#';
					}
					else if (i == '4')
					{
						character = '$';
					}
					else if (i == '5')
					{
						character = '%';
					}
					else if (i == '6')
					{
						character = '^';
					}
					else if (i == '7')
					{
						character = '&';
					}
					else if (i == '8')
					{
						character = '*';
					}
					else if (i == '9')
					{
						character = '(';
					}
					else if (i == '0')
					{
						character = ')';
					}
				}
				else
				{
					character = tolower(character);
				}

				// Update the current line with the new character
				this->mpState->mInput.currentLine += character;
				
			}
			break;
	}

}

/** Author: Dustin Yost
 * Called when some input has been entered (ENTER has been pressed)
 */
void StateServer::onInput(std::string &input)
{
	std::cout << input << '\n';
}

/** Author: Dustin Yost
 * Starts the server at whatever address and port are in the state data
 */
void StateServer::start()
{
	this->mPlayerAddressesLength = this->mpState->mNetwork.maxClients;
	this->mpPlayerAddresses = new PlayerAddress[mPlayerAddressesLength];
	for (int i = 0; i < this->mpState->mNetwork.maxClients; i++)
		this->mpPlayerAddresses[i] = NULL;

	ChampNetPlugin::StartServer(this->mpState->mNetwork.port, this->mpState->mNetwork.maxClients);
}

/** Author: Dustin Yost
 * Disconnects server and notifies clients of disconnection
 */
void StateServer::disconnect()
{
	this->sendDisconnectPacket(NULL, true);
	ChampNetPlugin::Disconnect();
}

/** Author: Dustin Yost
 * Sends clients the notification of server disconnection
 */
void StateServer::sendDisconnectPacket(const char *address, bool broadcast)
{
	PacketGeneral packet[1];
	packet->id = ChampNetPlugin::MessageIDs::ID_DISCONNECT;
	this->sendPacket(address == NULL ? ChampNetPlugin::GetAddress() : address, packet, broadcast);
}

/** Author: Dustin Yost
 * Updates the network
 */
void StateServer::updateNetwork()
{
	// Fetch all packets and put them in a queue - aka pull all packets from RakNet and put them in a queue in the plugin
	ChampNetPlugin::FetchPackets();
	// Iterate over all packets in the plugin queue
	void* pPacket = NULL;
	bool foundValidPacket = false;
	do
	{
		// Get the next available packet
		pPacket = ChampNetPlugin::PollPacket(foundValidPacket);
		// Check if that packet is valid (there was a packet in the queue)
		if (foundValidPacket)
		{
			// A packet was found, handle it
			this->handlePacket((ChampNet::Packet*)pPacket);
			// Free the packet from memory (delete it)
			ChampNetPlugin::FreePacket(pPacket);
		}
		// Only operate while there are packets left in this fetch (more may have come in since fetch, but they can be taken care of next call)
	} while (foundValidPacket);
	
}

/** Author: Dustin Yost
 * Handles the usage of all the different packet identifiers
 */
void StateServer::handlePacket(ChampNet::Packet *packet)
{
	// Check which packet type it is
	switch (packet->getID())
	{
		// Some client is connecting (expect a ID_USER_JOINED shortly)
		case ChampNetPlugin::ID_CLIENT_CONNECTION:
			{
				std::cout << "User connected\n";
				this->mpState->mNetwork.peersConnected++;
			}
			break;
		// Some client has disconnected
		case ChampNetPlugin::ID_CLIENT_DISCONNECTION:
			{
				std::cout << "User disconnected\n";
				this->mpState->mNetwork.peersConnected--;
			}
			break;
		// A client is joining
		case ChampNetPlugin::ID_USER_JOINED:
			{

				unsigned int pPacketLength = 0;
				PacketGeneral* pPacket = packet->getPacketAs<PacketGeneral>(pPacketLength);

				std::string addressSender = packet->getAddress();

				// Generate ID for user
				int id = this->findNextPlayerID();
				if (id < 0)
				{
					// Some invalid ID was found
					std::cout << "ERROR: Server is full, but another user has connected - disconnecting new user\n";
					this->sendDisconnectPacket(addressSender.c_str(), false);
					return;
				}
				// Set the id in the list with the new address
				this->mpPlayerAddresses[id] = new std::string(addressSender);

				// Print out that a user exists
				std::cout << "User " << id << " has joined from " << addressSender << '\n';

				// Create the packet to tell all peers of the user who joined
				PacketUserID packetID[1];
				// Set the new ID of the incoming user
				packetID->playerId = id;

				// Tell other players of new player
				packetID->id = ChampNetPlugin::ID_USER_SPAWN;
				this->sendPacket(addressSender.c_str(), packetID, true);

				// Tell user their player ID
				packetID->id = ChampNetPlugin::ID_USER_ID;
				this->sendPacket(addressSender.c_str(), packetID, false);
				
			}
			break;
		case ChampNetPlugin::ID_USER_UPDATE_POSITION:
			// A user's position/rotation is being updated
			{
				unsigned int pPacketLength = 0;
				PacketPlayerPosition* pPacket = packet->getPacketAs<PacketPlayerPosition>(pPacketLength);
				// Forward the packet along to all clients except the sender
				this->sendPacket(packet->getAddress().c_str(), pPacket, true);
			}
			break;
		case ChampNetPlugin::ID_USER_LEFT:
			// A user is leaving / has left the server
			{
				unsigned int pPacketLength = 0;
				PacketUserID* pPacket = packet->getPacketAs<PacketUserID>(pPacketLength);

				// Remove the address from the player's list
				delete this->mpPlayerAddresses[pPacket->playerId];
				this->mpPlayerAddresses[pPacket->playerId] = NULL;

				// Report out that the user left
				std::cout << "User " << pPacket->playerId << " has left\n";

				// Notify all other clients that the user left
				this->sendPacket(packet->getAddress().c_str(), pPacket, true);
			}
			break;
		case ChampNetPlugin::ID_BATTLE_REQUEST:
			// User (playerIdSender) is requesting User (playerIdReceiver) to battle
			{
				unsigned int pPacketLength = 0;
				PacketUserIDDouble* pPacket = packet->getPacketAs<PacketUserIDDouble>(pPacketLength);

				// Get the address of the sender (the challenger/requester) for reporting purposes
				std::string addressSender = packet->getAddress();
				// Get the address of the receiver
				std::string *addressReceiver = this->mpPlayerAddresses[pPacket->playerIdReceiver];
				// Report out the request
				std::cout << "Forwarding battle request from "
					<< pPacket->playerIdSender << " at " << addressSender << " to "
					<< pPacket->playerIdReceiver << " at " << addressReceiver << '\n';
				// Send the request to the receiver
				this->sendPacket(addressReceiver->c_str(), pPacket, false);
			}
			break;
		case ChampNetPlugin::ID_BATTLE_RESPONSE:
			// User (playerIdSender) is responding to User (playerIdReceiver) to battle
			{
				unsigned int pPacketLength = 0;
				PacketBattleResponse* pPacket = packet->getPacketAs<PacketBattleResponse>(pPacketLength);

				// Get the address of the receiver (the challenger/requester)
				const char *addressReceiver = this->mpPlayerAddresses[pPacket->playerIdReceiver]->c_str();
				// Forward the packet - forward the response to the one who requested the battle
				this->sendPacket(addressReceiver, pPacket, false);

				// TODO: This will be moved to post-battle
				if (pPacket->accepted)
				{
					// Notify all users of a battle winner
					PacketUserIDTriple packetBattleResult[1];
					packetBattleResult->id = ChampNetPlugin::ID_BATTLE_RESULT;
					// Rearrange the ids so it is valid with who requested the battle
					packetBattleResult->playerIdSender = pPacket->playerIdReceiver;
					packetBattleResult->playerIdReceiver = pPacket->playerIdSender;
					// get a random winner
					packetBattleResult->playerIdThird = rand() % 2 == 0 ? pPacket->playerIdReceiver : pPacket->playerIdSender;
					this->sendPacket(ChampNetPlugin::GetAddress(), packetBattleResult, true);
				}

			}
			break;
		default:
			std::cout << "Received packet with id " << packet->getID() << " with length " << packet->getDataLength() << '\n';
			break;
	}
}

void StateServer::render()
{

	// The current line being typed
	std::string input = this->mpState->mInput.currentLine;
	// The previous size of the line being typed
	// will be the same as input.length() when no input has changed
	// will be -1 if the line was inputted this update
	// will be input.length() - 1 if there was a letter added this update
	// will be input.length() + 1 if there was a letter removed this update
	unsigned int inputSizePrev = this->mpState->mInput.lineSizePrevious;

	// The current position of the cursor (the next letter will be written here)
	int cursorPosX = this->mpState->mConsole.cursorPosX;
	int cursorPosY = this->mpState->mConsole.cursorPosY;

}

/** Author: Dustin Yost
 * Send packet data to the network
 */
void StateServer::sendPacket(const char *address, char *data, int dataSize, bool broadcast)
{
	PacketPriority priority = HIGH_PRIORITY;
	PacketReliability reliability = RELIABLE;
	ChampNetPlugin::SendData(address, data, dataSize, &priority, &reliability, 0, broadcast);
}

/** Author: Dustin Yost
 * Finds the next available address slot, returning -1 if none is found
 */
int StateServer::findNextPlayerID()
{
	for (int i = 0; i < this->mpState->mNetwork.maxClients; i++)
	{
		if (this->mpPlayerAddresses[i] == NULL) return i;
	}
	return -1;
}
