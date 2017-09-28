#include "Steering\SteeringGroupAlignment.h"

#include "Units\Unit.h"
#include "Steering\SteeringMatchVelocity.h"
#include "Steering\AlignSteering.h"

#include "Util\Game.h"
#include "Units\UnitManager.h"
#include <cassert>

using std::vector;

SteeringGroupAlignment::SteeringGroupAlignment() : SteeringNeighborhood() {
	mpMatchVelocity = new SteeringMatchVelocity();
	//mpAlign = new AlignSteering();
}

SteeringGroupAlignment::SteeringGroupAlignment(const SteeringData& data) : SteeringGroupAlignment() {
	this->setData(data);
}

void SteeringGroupAlignment::setData(const SteeringData& data) {
	SteeringNeighborhood::setData(data);

	SteeringData subData = SteeringData(data);

	// Match: Velocity
	subData.type = this->mpMatchVelocity->getType();
	this->mpMatchVelocity->setData(subData);
	// Match: Alignment
	//subData.type = this->mpAlign->getType();
	//this->mpAlign->setData(subData);

}

SteeringGroupAlignment::~SteeringGroupAlignment() {
	delete mpMatchVelocity;
	//delete mpAlign;
}

Steering* SteeringGroupAlignment::getSteering()
{
	Unit* pOwner = this->getUnit();
	assert(pOwner != NULL);

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	if (mShouldCalculateNeighborhood) {
		this->calculateNeighborhood();
	}

	//data.rotAcc = mpAlign->getSteering()->getData().rotAcc;

	// Find the average velocity and rotation for delegated behaviors
	Vector2D averageVelocity = ZERO_VECTOR2D;

	int neighborCount = mNeighborhood.size();
	if (neighborCount == 0) {
		this->mData = data;
		return this;
	}

	Unit* pNeighbor;
	for (int i = 0; i < neighborCount; i++) {
		pNeighbor = mNeighborhood.at(i);
		averageVelocity += pNeighbor->getPhysicsComponent()->getData().vel;
	}
	averageVelocity /= (float)neighborCount;

	// Arbitration:
	// Get acc and rotAcc from delegated behaviors
	data.acc = mpMatchVelocity->getSteeringFor(averageVelocity)->getData().acc;

	this->mData = data;

	return this;
}
