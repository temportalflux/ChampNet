#pragma once

#include "Steering\Steering.h"

class ArriveSteering : public Steering
{

protected:
	// The radius around the target at which the owner should slow down
	float mRadiusArriving;
	// The radius around the target at which the owner is considered "arrived"
	float mRadiusArrived;

public:
	ArriveSteering();
	ArriveSteering(const SteeringData& data);

	virtual void setData(const SteeringData& data);

	virtual Steering* getSteering();

};
