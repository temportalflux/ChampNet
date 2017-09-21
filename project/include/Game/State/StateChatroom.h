/*
Dustin Yost 0984932
Jon Trusheim 0979380
EGP-405-02
Project 1
09/21/2017
We certify that this work is entirely our own.
The assessor of this project may reproduce this project and provide
copies to other academic staff, and/or communicate a copy of this project
to a plagiarism-checking service, which may retain a copy of
the project on its database.
*/

#pragma once

#include "Game\State\StateApplication.h"
#include <string>
#include "Game\Network\MessageHandler.h"
#include <deque>

class Framework;
struct FrameworkData;

struct PacketString;
struct PacketChatMessage;
struct PacketUInt;
struct PacketStringLarge;
struct PacketStringLargeUsername;
struct PacketUsername;

class StateChatroom abstract : public StateApplication, public MessageHandler {

private:
	Framework* mpNetworkFramework;
	std::deque<Network::PacketInfo*> mPacketInputQueue;

	void updateNetwork();

protected:
	void updateGame();
	
	virtual void onEnterFrom(StateApplication *previous);

	/* Author: Dustin Yost
	Handles the queue of packets to later be processed
	*/
	virtual void handlePacket(Network::PacketInfo *info) override;

	/* Author: Dustin Yost
		Handle packets which have been recieved since last update
	*/
	virtual void doHandlePacket(Network::PacketInfo *info) = 0;

	/* Author: Dustin Yost
		Pushes a text message to the record of messages to display
	*/
	void pushMessage(const std::string &msg);


	/*
	Author: Jon Trusheim
	chat commands
	*/
	bool chatCommands(const std::string &latestLine);

	Framework* getFramework();

	virtual void sendMessage(const std::string &message);
	virtual void sendMessage(const std::string &username, const std::string &message);

	virtual void sendToServer(PacketString *packet) = 0;
	virtual void sendToServer(PacketChatMessage *packet) = 0;
	virtual void sendToServer(PacketStringLarge *packet) = 0;
	virtual void sendToServer(PacketUsername *packet) = 0;
	virtual void sendToServer(PacketUInt *packet) = 0;

public:
	StateChatroom();
	virtual ~StateChatroom();

};
