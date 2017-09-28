#pragma once

#include "Steering\SeekSteering.h"

class SteeringEnemyFind : public SeekSteering
{

protected:
	virtual Steering* getSteering();

public:
	SteeringEnemyFind();
	SteeringEnemyFind(const SteeringData& data);

};