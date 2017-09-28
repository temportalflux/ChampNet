#pragma once

#include "Steering\ArriveAndFaceSteering.h"

class WanderSteering : public ArriveAndFaceSteering
{

protected:
	// The minimum distance the unit can be to the target before changing targets
	float mDistanceWanderSq;

	virtual Steering* getSteering();
	void findNextTarget(Vector2D &out);

public:
	WanderSteering();
	WanderSteering(const SteeringData& data);
	
	virtual void setData(const SteeringData& data);

};
