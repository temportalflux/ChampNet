#ifndef CHAMPNET_SERVER_SERVER_H
#define CHAMPNET_SERVER_SERVER_H

#include "StateApplication.h"

#include <string>

class StateServer : public StateApplication
{

public:

	StateServer();
	virtual ~StateServer();

	virtual void onEnterFrom(StateApplication *previous) override;

	/** Author: Dustin Yost
	* Called when a key is marked as down this update
	*/
	virtual void onKeyDown(int i) override;

	/** Author: Dustin Yost
	* Called when some input has been entered (ENTER has been pressed)
	*/
	virtual void onInput(std::string &input);
	
	virtual void render() override;

	void start();
	void disconnect();

};

#endif // CHAMPNET_SERVER_SERVER_H