#pragma once

#include "Steering\Steering.h"

class SteeringMatchAlignment : public Steering
{
	friend class SteeringGroupAlignment;

protected:
	virtual Steering* getSteering();

public:
	SteeringMatchAlignment();

	virtual Steering* getSteeringFor(float velocity);

};
