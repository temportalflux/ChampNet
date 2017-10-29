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
	if (this->mUsedPlayerIDs != NULL)
	{
		// dont need to delete each, they are just strings
		delete[] this->mUsedPlayerIDs;
		this->mUsedPlayerIDs = NULL;
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

void StateServer::start()
{
	this->mUsedPlayerIDs = new const char*[this->mpState->mNetwork.maxClients];
	for (int i = 0; i < this->mpState->mNetwork.maxClients; i++)
		this->mUsedPlayerIDs[i] = NULL;

	ChampNetPlugin::StartServer(this->mpState->mNetwork.port, this->mpState->mNetwork.maxClients);
}

void StateServer::disconnect()
{
	this->sendDisconnectPacket(NULL, true);
	ChampNetPlugin::Disconnect();
}

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
	ChampNetPlugin::FetchPackets();
	void* pPacket = NULL;
	bool foundValidPacket = false;
	do
	{
		pPacket = ChampNetPlugin::PollPacket(foundValidPacket);
		if (foundValidPacket)
		{
			this->handlePacket((ChampNet::Packet*)pPacket);
			ChampNetPlugin::FreePacket(pPacket);
		}
	} while (foundValidPacket);
	
}

void StateServer::handlePacket(ChampNet::Packet *packet)
{
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

				// TODO: Clients need to send messages with their ID when they leave
				//PacketUserID packetLeft[1];
				//packetLeft->id = ChampNetPlugin::ID_USER_LEFT;
				//packetLeft->playerId = 


			}
			break;
		// A client is joining
		case ChampNetPlugin::ID_USER_JOINED:
			{

				unsigned int pPacketLength = 0;
				PacketGeneral* pPacket = packet->getPacketAs<PacketGeneral>(pPacketLength);

				const char *addressSender;
				unsigned int addressLength;
				packet->getAddress(addressSender, addressLength);

				// Generate ID for user
				int id = this->findNextPlayerID();
				if (id < 0)
				{
					std::cout << "ERROR: Server is full, but another user has connected - disconnecting new user\n";
					this->sendDisconnectPacket(addressSender, false);
					return;
				}
				this->mUsedPlayerIDs[id] = addressSender;

				std::cout << "User " << id << " has joined from " << addressSender << '\n';

				PacketUserID packetID[1];
				packetID->playerId = id;

				// Tell other players of new player
				packetID->id = ChampNetPlugin::ID_USER_SPAWN;
				this->sendPacket(addressSender, packetID, true);

				// Tell user their player ID
				packetID->id = ChampNetPlugin::ID_USER_ID;
				this->sendPacket(addressSender, packetID, false);
				
			}
			break;
		case ChampNetPlugin::ID_USER_UPDATE_POSITION:
			{
				unsigned int pPacketLength = 0;
				PacketPlayerPosition* pPacket = packet->getPacketAs<PacketPlayerPosition>(pPacketLength);

				const char *addressSender;
				unsigned int addressLength;
				packet->getAddress(addressSender, addressLength);

				//std::cout << "Passing along posUpdate(" << (int)pPacket->id << ") from " << pPacket->playerId << " at " << pPacket->posX << " | " << pPacket->posY << '\n';
				
				this->sendPacket(addressSender, pPacket, true);
			}
			break;
		case ChampNetPlugin::ID_USER_LEFT:
			{
				unsigned int pPacketLength = 0;
				PacketUserID* pPacket = packet->getPacketAs<PacketUserID>(pPacketLength);

				this->mUsedPlayerIDs[pPacket->playerId] = NULL;

				std::cout << "User " << pPacket->playerId << " has left\n";

				const char *addressSender;
				unsigned int addressLength;
				packet->getAddress(addressSender, addressLength);

				this->sendPacket(addressSender, pPacket, true);
			}
			break;
		case ChampNetPlugin::ID_BATTLE_REQUEST:
			{
				// User (playerIdSender) is requesting User (playerIdReceiver) to battle
				unsigned int pPacketLength = 0;
				PacketUserIDDouble* pPacket = packet->getPacketAs<PacketUserIDDouble>(pPacketLength);

				const char *addressSender;
				unsigned int addressLength;
				packet->getAddress(addressSender, addressLength);

				const char *addressReceiver = this->mUsedPlayerIDs[pPacket->playerIdReceiver];
				
				this->sendPacket(addressReceiver, pPacket, false);
			}
			break;
		case ChampNetPlugin::ID_BATTLE_RESPONSE:
			// User (playerIdSender) is responding to User (playerIdReceiver) to battle
			{
				unsigned int pPacketLength = 0;
				PacketBattleResponse* pPacket = packet->getPacketAs<PacketBattleResponse>(pPacketLength);

				const char *addressSender;
				unsigned int addressLength;
				packet->getAddress(addressSender, addressLength);

				const char *addressReceiver = this->mUsedPlayerIDs[pPacket->playerIdReceiver];

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

void StateServer::sendPacket(const char *address, char *data, int dataSize, bool broadcast)
{
	PacketPriority priority = HIGH_PRIORITY;
	PacketReliability reliability = RELIABLE;
	ChampNetPlugin::SendData(address, data, dataSize, &priority, &reliability, 0, broadcast);
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

int StateServer::findNextPlayerID()
{
	for (int i = 0; i < this->mpState->mNetwork.maxClients; i++)
	{
		if (this->mUsedPlayerIDs[i] == NULL) return i;
	}
	return -1;
}
