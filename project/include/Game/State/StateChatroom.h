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
struct PacketChatMessage;
struct PacketUInt;

class StateChatroom abstract : public StateApplication, public MessageHandler {

private:
	Framework* mpNetworkFramework;
	std::deque<Network::PacketInfo*> mPacketInputQueue;

	virtual void onEnterFrom(StateApplication *previous);
	void updateNetwork();
	void updateGame();

protected:
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
	void chatCommands(const std::string &latestLine);

	Framework* getFramework();

	virtual void sendMessage(const std::string &message);
	virtual void sendMessage(const std::string &username, const std::string &message);

	virtual void sendTo(PacketString packet) = 0;
	virtual void sendTo(PacketChatMessage packet) = 0;

public:
	StateChatroom();
	virtual ~StateChatroom();

};
