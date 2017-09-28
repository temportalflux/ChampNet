#pragma once

#include "Steering\Steering.h"

class SteeringMatchVelocity : public Steering
{
	friend class SteeringGroupAlignment;

protected:
	virtual Steering* getSteering();

public:
	SteeringMatchVelocity();

	virtual Steering* getSteeringFor(Vector2D velocity);

};
