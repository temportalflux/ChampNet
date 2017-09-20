#pragma once

#include "Game\State\StateApplication.h"
#include "Game\Network\MessageHandler.h"
#include <deque>

class Framework;
struct FrameworkData;

class StateChatroom abstract : public StateApplication, public MessageHandler {

private:
	Framework* mpNetworkFramework;
	std::deque<Network::PacketInfo> mPacketInputQueue;

	virtual void onEnterFrom(StateApplication *previous);
	void updateNetwork();
	void updateGame();

	virtual void handlePacket(Network::PacketInfo info) override;

protected:
	virtual void doHandlePacket(Network::PacketInfo info) = 0;

public:
	StateChatroom();
	virtual ~StateChatroom();

};
