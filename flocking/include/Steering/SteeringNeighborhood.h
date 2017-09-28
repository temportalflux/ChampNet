#pragma once

#include "Steering\Steering.h"

#include <vector>

class Unit;

class SteeringNeighborhood : public Steering
{
	friend class SteeringFlocking;

private:
	float mRadiusNeighborhood;

protected:
	std::vector<Unit*> mNeighborhood;
	bool mShouldCalculateNeighborhood;

public:
	SteeringNeighborhood();
	SteeringNeighborhood(const SteeringData& data);

	void setRadius(float radius);
	float getRadius();

	virtual void setDoCalculateNeighborhood(bool calculate) override;

	virtual void calculateNeighborhood(std::vector<Unit*>* filterList = NULL) override;

	std::vector<Unit*> calculateNeighborhood(Unit* owner, float radius, std::vector<Unit*>* filterList = NULL);

};
