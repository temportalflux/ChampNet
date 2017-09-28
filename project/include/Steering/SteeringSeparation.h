#pragma once

#include "Steering\SteeringNeighborhood.h"

class SeekSteering;

class SteeringSeparation : public SteeringNeighborhood
{

private:
	SeekSteering* mpSeek;

public:
	SteeringSeparation();
	SteeringSeparation(const SteeringData& data);
	virtual ~SteeringSeparation();
	virtual void setData(const SteeringData& data);

	virtual Steering* getSteering();

};
