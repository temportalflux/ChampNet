#pragma once

#include "Game\State\StateApplication.h"
#include <string>
class Framework;
struct FrameworkData;

class StateChatroom : public StateApplication {

private:
	Framework* mpNetworkFramework;

	virtual void onEnterFrom(StateApplication *previous);
	void updateNetwork();
	void updateGame();
	void render();
	

public:
	StateChatroom();
	virtual ~StateChatroom();

};
