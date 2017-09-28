#pragma once

#include "Steering\SteeringNeighborhood.h"

class SteeringMatchVelocity;
//class AlignSteering;

class SteeringGroupAlignment : public SteeringNeighborhood
{
	friend class SteeringFlocking;

private:
	SteeringMatchVelocity* mpMatchVelocity;
	//AlignSteering* mpAlign;

protected:
	virtual Steering* getSteering();

public:
	SteeringGroupAlignment();
	SteeringGroupAlignment(const SteeringData& data);
	virtual ~SteeringGroupAlignment();
	virtual void setData(const SteeringData& data) override;

};
