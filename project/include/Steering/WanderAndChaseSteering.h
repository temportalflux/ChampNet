#pragma once

#include "Steering\WanderSteering.h"

class WanderAndChaseSteering : public WanderSteering {

protected:
	// The radius at which the target unit should be seeked after
	float mRadiusSeekSq;

	virtual Steering* getSteering();

public:
	WanderAndChaseSteering();
	WanderAndChaseSteering(const SteeringData& data);

	virtual void setData(const SteeringData& data);

};
