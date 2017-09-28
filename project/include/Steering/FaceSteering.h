#pragma once

#include "Steering\AlignSteering.h"

class FaceSteering : public AlignSteering
{
	friend class ArriveAndFaceSteering;
	friend class WanderAndChaseSteering;

protected:

public:
	FaceSteering();
	FaceSteering(const SteeringData& data);

	virtual float getOrientationTarget(Unit* pOwner);

	virtual Steering* getSteering();

};
