#pragma once

class State {

public:
	virtual ~State();

	virtual void init() = 0;

};
