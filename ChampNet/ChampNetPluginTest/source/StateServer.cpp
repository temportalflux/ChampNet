/*
Names and ID: Christopher Brennan: 1028443, Dustin Yost: 0984932, Jacob Ruth: 0890406
Course Info: EGP-405-01 
Project Name: Project 3: Synchronized Networking
Due: 11/22/17
Certificate of Authenticity (standard practice): “We certify that this work is entirely our own.  
The assessor of this project may reproduce this project and provide copies to other academic staff, 
and/or communicate a copy of this project to a plagiarism-checking service, which may retain a copy of the project on its database.”
*/

#include "StateServer.h"

#include "ChampNetPlugin.h"
#include "Packet.h"

#include "StateData.h"
#include "Win.h"
#include <iostream>
#include <RakNet\PacketPriority.h>
#include "Packets.h"
#include "GameState.h"
#include "PerformanceTracker.h"

#include <stdlib.h> // rand
#include <sstream>
#include <time.h>

StateServer::StateServer() : StateApplication()
{
	ChampNetPlugin::Create();

	this->mpGameState = new GameState(-1);
	this->mpTimers = new PerformanceTracker();
	this->mMsPerUpdate = 1 / (10 * 0.001); // (10 updates / second) * (seconds / ms) = (updates / ms) => (u/ms)^(-1) = (ms/u)
}

StateServer::~StateServer()
{
	if (this->mpClientAddresses != NULL)
	{
		for (int i = 0; i < mClientCount; i++)
			if (mpClientAddresses[i] != NULL)
			{
				delete mpClientAddresses[i];
				mpClientAddresses[i] = NULL;
			}
		delete[] this->mpClientAddresses;
		this->mpClientAddresses = NULL;
	}

	if (this->mpPlayerIdToClientId != NULL)
	{
		delete[] this->mpPlayerIdToClientId;
		this->mpPlayerIdToClientId = NULL;
	}

	if (this->mpClientIdToPlayers != NULL)
	{
		for (int i = 0; i < mClientCount; i++)
			if (mpClientIdToPlayers[i] != NULL)
			{
				delete mpClientIdToPlayers[i];
				mpClientIdToPlayers[i] = NULL;
			}
		delete[] this->mpClientIdToPlayers;
		this->mpClientIdToPlayers = NULL;
	}

	if (mpGameState != NULL)
	{
		delete mpGameState;
		mpGameState = NULL;
	}

	if (this->mpTimers != NULL)
	{
		delete mpTimers;
		this->mpTimers = NULL;
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
	std::cout << "Starting server...\n";

	this->mClientCount = this->mpState->mNetwork.maxClients;
	this->mpClientAddresses = new PlayerAddress[mClientCount];
	for (int i = 0; i < mClientCount; i++)
		this->mpClientAddresses[i] = NULL;

	this->mPlayerCount = this->mpState->mNetwork.maxClients * this->mpState->mNetwork.maxPlayersPerClient;
	this->mpPlayerIdToClientId = new int[mPlayerCount];
	for (int i = 0; i < mPlayerCount; i++)
		this->mpPlayerIdToClientId[i] = -1;

	this->mpClientIdToPlayers = new int*[mClientCount];
	for (int i = 0; i < mClientCount; i++)
		this->mpClientIdToPlayers[i] = NULL;

	ChampNetPlugin::StartServer(this->mpState->mNetwork.port, this->mpState->mNetwork.maxClients);

	this->mpTimers->startTracking("update");
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

				// Remove client
				//this->removeClient(this->cl)
			}
			break;
			// A client is joining
		case ChampNetPlugin::ID_CLIENT_JOINED:
			{

				unsigned int pPacketLength = 0;
				PacketGeneral* pPacket = packet->getPacketAs<PacketGeneral>(pPacketLength);

				int playerRequestCount;
				PlayerRequest *pPlayerRequests = NULL;
				this->deserializePlayerRequests(packet, pPlayerRequests, playerRequestCount);

				std::string addressSender = packet->getAddress();

				unsigned int clientID;
				if (!this->addClient(addressSender.c_str(), clientID))
				{
					this->sendDisconnectPacket(addressSender.c_str(), false);
					return;
				}

				// Print out that a user exists
				std::cout << "Client " << clientID << " has joined from " << addressSender << '\n';

				for (int i = 0; i < playerRequestCount; i++)
				{
					unsigned int playerID;
					if (!this->addPlayer(clientID,
						pPlayerRequests[i].localID, playerID,
						pPlayerRequests[i].name,
						pPlayerRequests[i].colorR, pPlayerRequests[i].colorG, pPlayerRequests[i].colorB
					))
					{
						this->sendDisconnectPacket(addressSender.c_str(), false);
						std::cout << "Removing client " << clientID << '\n';
						this->removeClient(clientID);
						return;
					}
				}

				// Tell user their client/player ID
				this->sendGameState(ChampNetPlugin::ID_UPDATE_GAMESTATE, addressSender.c_str(), false, clientID);
				// Tell other players of new player
				//this->sendGameState(ChampNetPlugin::ID_UPDATE_GAMESTATE, addressSender.c_str());

				delete[] pPlayerRequests;
			}
			break;
		case ChampNetPlugin::ID_CLIENT_LEFT:
			// A user is leaving / has left the server
			{
				unsigned int pPacketLength = 0;
				PacketUserID* pPacket = packet->getPacketAs<PacketUserID>(pPacketLength);
				unsigned int clientID = pPacket->dataID;

				// Report out that the user left
				std::cout << "Client " << clientID << " has left\n";

				this->removeClient(clientID);

				// Notify all other clients that the user left
				this->sendPacket(packet->getAddress().c_str(), pPacket, true);
			}
			break;
		case ChampNetPlugin::ID_PLAYER_REQUEST_MOVEMENT:
			// A user's position/rotation is being updated
			{
				unsigned int pPacketLength = 0;
				PacketPlayerPosition* pPacket = packet->getPacketAs<PacketPlayerPosition>(pPacketLength);
				unsigned int clientID = pPacket->clientID;
				unsigned int playerID = pPacket->playerID;

				// Integrate the position change into the gamestate
				this->mpGameState->players[playerID].posX = pPacket->posX;
				this->mpGameState->players[playerID].posY = pPacket->posY;
				this->mpGameState->players[playerID].velY = pPacket->velY;
				this->mpGameState->players[playerID].velX = pPacket->velX;

				//std::cout << "Integrated " << this->mpGameState->players[playerID].posX << " " << this->mpGameState->players[playerID].posY << '\n';

				// ship gamestate back to all clients
				//this->sendGameState(ChampNetPlugin::ID_UPDATE_GAMESTATE);
			}
			break;
		case ChampNetPlugin::ID_CLIENT_REQUEST_PLAYER:
			{
				std::string addressSender = packet->getAddress();

				// The client wants to spawn another player
				unsigned int pPacketLength = 0;
				PacketClientPlayerID* pPacket = packet->getPacketAs<PacketClientPlayerID>(pPacketLength);
				unsigned int clientID = pPacket->clientID;
				unsigned int localID = pPacket->playerID;

				std::cout << "Client " << clientID <<
					" has requested another player for localID " << localID << '\n';

				// Get the next playerID
				unsigned int playerID;
				if (!this->addPlayer(clientID, localID, playerID, "", 0, 0, 0))
				{
					std::cout << "Could not provide another player to client|local="
						<< clientID << "|" << localID << '\n';
					return;
				}

				//this->sendGameState(ChampNetPlugin::ID_UPDATE_GAMESTATE);
			}
			break;
		case ChampNetPlugin::ID_CLIENT_MISSING:
			std::cout << "Connection lost\n";
			this->stopRunning();
			break;
			/*
		
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
			//*/
		case ChampNetPlugin::ID_CLIENT_SCORE_UP: // used to increment a client score by 1 // proof that scoreboard works
			{

				std::string addressSender = packet->getAddress();

				unsigned int pPacketLength = 0;

				PacketPlayerScoreboardInfo* pPacket = packet->getPacketAs<PacketPlayerScoreboardInfo>(pPacketLength);
				unsigned int clientID = pPacket->clientID;
				unsigned int playerID = pPacket->playerID;

				// Integrate the win and increment by 1 into the gamestate
				this->mpGameState->players[playerID].wins = (pPacket->score + 1);

				//this->sendGameState(ChampNetPlugin::ID_UPDATE_GAMESTATE, addressSender.c_str(), true, clientID);
			}
		break;
		case ChampNetPlugin::ID_CLIENT_RANK_INTEGRATION:
		{
			std::string addressSender = packet->getAddress();

			unsigned int pPacketLength = 0;

			PacketPlayerScoreboardInfo* pPacket = packet->getPacketAs<PacketPlayerScoreboardInfo>(pPacketLength);
			unsigned int clientID = pPacket->clientID;
			unsigned int playerID = pPacket->playerID;

			// Integrate the win and increment by 1 into the gamestate
			this->mpGameState->players[playerID].rank = (pPacket->score);

			//this->sendGameState(ChampNetPlugin::ID_UPDATE_GAMESTATE, addressSender.c_str(), true, clientID);
		}
		break;
		default:
			std::cout << "Received packet with id " << packet->getID() << " with length " << packet->getDataLength() << '\n';
			break;
	}
}

void StateServer::updateGame()
{
	StateApplication::updateGame();

	// get update time in MS
	if (this->mpTimers->getElapsedTime("update") >= this->mMsPerUpdate)
	{
		this->sendGameState(ChampNetPlugin::MessageIDs::ID_UPDATE_GAMESTATE);
		//std::cout << "Update " << time(NULL) << '\n';
		this->mpTimers->stopTracking("update");
		this->mpTimers->clearTracker("update");
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
bool StateServer::findNextClientID(unsigned int &id)
{
	for (unsigned int i = 0; i < this->mpState->mNetwork.maxClients; i++)
	{
		if (this->mpClientAddresses[i] == NULL)
		{
			id = i;
			return true;
		}
	}
	return false;
}

/** Author: Dustin Yost
* Finds the next available player slot, returning -1 if none is found
*/
bool StateServer::findNextPlayerID(unsigned int &id)
{
	for (int i = 0; i < this->mPlayerCount; i++)
	{
		if (this->mpPlayerIdToClientId[i] < 0)
		{
			id = i;
			return true;
		}
	}
	return false;
}

bool StateServer::addClient(const char* address, unsigned int &id)
{
	// Generate ID for user
	if (!this->findNextClientID(id))
	{
		// Some invalid ID was found
		std::cout << "ERROR: Server is full, but another user has connected - disconnecting new user\n";
		return false;
	}
	// Set the id in the list with the new address
	this->mpClientAddresses[id] = new std::string(address);
	return true;
}

void StateServer::removeClient(unsigned int id)
{
	if (this->mpClientAddresses[id] != NULL)
	{
		delete this->mpClientAddresses[id];
		this->mpClientAddresses[id] = NULL;
	}

	if (this->mpClientIdToPlayers[id] != NULL)
	{
		// Remove all players
		for (unsigned int localID = 0; localID < this->mpState->mNetwork.maxPlayersPerClient; localID++)
		{
			if (this->mpClientIdToPlayers[id][localID] >= 0)
			{
				unsigned int playerID = this->mpClientIdToPlayers[id][localID];
				std::cout << "Removing player " << playerID
					<< " at client|local=" << id << '|' << localID << '\n';
				this->mpGameState->removePlayer(playerID);
				this->mpPlayerIdToClientId[playerID] = -1;
			}
		}

		// Remove map
		delete this->mpClientIdToPlayers[id];
		this->mpClientIdToPlayers[id] = NULL;
	}
}

bool StateServer::addPlayer(unsigned int clientID, unsigned int localID, unsigned int &playerID,
	std::string name, float colorR, float colorG, float colorB)
{
	if (!this->findNextPlayerID(playerID))
	{
		// Some invalid ID was found
		std::cout << "ERROR: Server is full, but another player has connected - disconnecting client\n";
		return false;
	}
	this->mpPlayerIdToClientId[playerID] = clientID;
	
	if (this->mpClientIdToPlayers[clientID] == NULL)
	{
		this->mpClientIdToPlayers[clientID] = new int[this->mpState->mNetwork.maxPlayersPerClient];
		for (unsigned int i = 0; i < this->mpState->mNetwork.maxPlayersPerClient; i++)
			this->mpClientIdToPlayers[clientID][i] = -1;
	}
	this->mpClientIdToPlayers[clientID][localID] = playerID;

	this->mpGameState->addPlayer(clientID, playerID, localID, name, colorR, colorG, colorB);

	return true;
}

void StateServer::sendGameState(unsigned char msgID, const char* sender, bool broadcast, int clientID)
{
	int dataLength;
	char *data = this->mpGameState->serializeForClient(msgID, clientID, dataLength);
	this->sendPacket(sender == NULL ? ChampNetPlugin::GetAddress() : sender, data, dataLength, broadcast);
	delete data;
}

void StateServer::deserializePlayerRequests(ChampNet::Packet *pPacket, PlayerRequest *&requests, int &playerRequestCount)
{
	unsigned int pPacketLength = 0;
	unsigned char *data;
	pPacket->getData(data, pPacketLength);
	unsigned char *dataHead = data;

	unsigned char packetID = *((unsigned char *)dataHead);
	dataHead += sizeof(unsigned char);
	
	playerRequestCount = *((int *)dataHead);
	dataHead += sizeof(int);

	requests = new PlayerRequest[playerRequestCount];

	for (int localID = 0; localID < playerRequestCount; localID++)
	{
		requests[localID] = PlayerRequest();
		requests[localID].localID = localID;

		int nameLength = *((int *)dataHead);
		dataHead += sizeof(int);

		std::stringstream nameStream("");
		for (int i = 0; i < nameLength; i++)
		{
			nameStream << *((char *)dataHead); dataHead += sizeof(char) * 2; // to account for c# UTF-16 encoding
		}
		requests[localID].name = nameStream.str();

		requests[localID].colorR = *((float *)dataHead); dataHead += sizeof(float);
		requests[localID].colorG = *((float *)dataHead); dataHead += sizeof(float);
		requests[localID].colorB = *((float *)dataHead); dataHead += sizeof(float);
	}

}
