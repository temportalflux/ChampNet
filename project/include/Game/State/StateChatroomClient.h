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

#include "Game\State\StateChatroom.h"
#include "Network\Framework.h"

class StateChatroomClient : public StateChatroom {

private:
	RakNet::SystemAddress *mAddressServer;

	void updateGame() override;
	void doHandlePacket(Network::PacketInfo *info) override;
	void render();
	
	virtual void sendToServer(PacketString *packet) override;
	virtual void sendToServer(PacketChatMessage *packet) override;
	virtual void sendToServer(PacketStringLarge *packet) override;
	virtual void sendToServer(PacketUsername *packet) override;
	virtual void sendToServer(PacketUInt *packet) override;

};
