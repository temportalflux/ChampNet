#pragma once

#include "Steering\ArriveSteering.h"
#include "Steering\FaceSteering.h"

class ArriveAndFaceSteering : public ArriveSteering
{

protected:
	FaceSteering mSteeringFace;

	virtual Steering* getSteering();

public:
	ArriveAndFaceSteering();
	ArriveAndFaceSteering(const SteeringData& data);

	virtual void setData(const SteeringData& data);

};
