#pragma once

#include "Steering\SteeringNeighborhood.h"

class SeekSteering;

class SteeringCohesion : public SteeringNeighborhood
{

private:
	SeekSteering* mpSeek;

public:
	SteeringCohesion();
	SteeringCohesion(const SteeringData& data);
	virtual ~SteeringCohesion();
	virtual void setData(const SteeringData& data);

	virtual Steering* getSteering();
};
