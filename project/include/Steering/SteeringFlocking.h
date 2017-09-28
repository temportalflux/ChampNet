#pragma once

#include "Steering\SteeringWeighted.h"

class SteeringNeighborhood;
class AlignSteering;

class SteeringFlocking : public SteeringWeighted
{

private:
	SteeringNeighborhood* mpNeighborhoodTracker;
	AlignSteering* mpFace;

protected:
	virtual Steering* getSteering();

public:
	SteeringFlocking();
	SteeringFlocking(const SteeringData& data);
	virtual ~SteeringFlocking();

	virtual void setData(const SteeringData& data) override;

	virtual void getWeightData(DataPoint *dataRoot, DataPoint *&out) override;
	virtual void loadMiscData(DataPoint *weightRoot) override;

};
