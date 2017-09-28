#pragma once

#include "Steering\Steering.h"

class AlignSteering : public Steering
{

protected:

public:
	AlignSteering();
	AlignSteering(const SteeringData& data);

	virtual float getOrientationTarget(Unit* pOwner);

	virtual Steering* getSteering();

};
