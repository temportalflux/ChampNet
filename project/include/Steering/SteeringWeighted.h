#pragma once

#include "Steering\Steering.h"

#include <deque>

class SteeringWeighted : public Steering
{

private:

protected:
	std::deque<Steering *> mSteerings;

	// Handle weighted blending for child steerings
	Steering* addWeightedSteering(Steering *steering);

public:
	SteeringWeighted();
	SteeringWeighted(const SteeringData& data);
	virtual ~SteeringWeighted();

	virtual void setData(const SteeringData& data) override;

	virtual void getWeightData(DataPoint *dataRoot, DataPoint *&out);
	virtual void onDataChanged() override;
	virtual void loadMiscData(DataPoint *weightRoot);

	virtual Steering* getSteering();

};
