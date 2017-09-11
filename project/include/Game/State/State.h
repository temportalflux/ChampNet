#pragma once

class State {

protected:
	virtual ~State();

	virtual void init() = 0;

};
