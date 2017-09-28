#pragma once

#include <Trackable.h>
#include "Steering\Steering.h"

class SeekSteering : public Steering
{

protected:
	bool mShouldFlee;

public:
	SeekSteering(bool shouldFlee = false);
	SeekSteering(const SteeringData& data, bool shouldFlee = false);
	virtual Steering* getSteering();

};