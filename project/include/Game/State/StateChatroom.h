#pragma once

#include "Game\State\StateApplication.h"
#include <string>
#include "Game\Network\MessageHandler.h"
#include <deque>

class Framework;
struct FrameworkData;
struct Packet;

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

	Framework* getFramework();

	virtual void sendMessage(const std::string &message);
	virtual void sendMessage(const std::string &username, const std::string &message);
	virtual void sendPacket(Packet packet) = 0;

public:
	StateChatroom();
	virtual ~StateChatroom();

};
