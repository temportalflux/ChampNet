#ifndef CHAMPNET_SERVER_SERVER_H
#define CHAMPNET_SERVER_SERVER_H

#include "StateApplication.h"

#include <string>

class StateServer : public StateApplication
{

public:

	StateServer();
	virtual ~StateServer();

	/** Author: Dustin Yost
	* Called when a key is marked as down this update
	*/
	virtual void onKeyDown(int i) override;

	/** Author: Dustin Yost
	* Called when some input has been entered (ENTER has been pressed)
	*/
	virtual void onInput(std::string &input);
	
	void start();
	void disconnect();

	virtual void render() override;

};

#endif // CHAMPNET_SERVER_SERVER_H