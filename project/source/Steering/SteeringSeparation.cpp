#include "Steering\SteeringSeparation.h"

#include "Units\Unit.h"
#include "Steering\SeekSteering.h"

#include "Util\Game.h"
#include "Units\UnitManager.h"
#include <cassert>

using std::vector;

SteeringSeparation::SteeringSeparation() : SteeringNeighborhood() {
	mpSeek = new SeekSteering(true);
	mShouldCalculateNeighborhood = true;
}

SteeringSeparation::SteeringSeparation(const SteeringData& data) : SteeringSeparation() {
	this->setData(data);
}

void SteeringSeparation::setData(const SteeringData& data) {
	SteeringNeighborhood::setData(data);

	SteeringData subData = SteeringData(data);

	// Arrive
	subData.type = this->mpSeek->getType();
	this->mpSeek->setData(subData);

}

SteeringSeparation::~SteeringSeparation() {
	delete mpSeek;
}

Steering* SteeringSeparation::getSteering()
{
	Unit* pOwner = this->getUnit();
	assert(pOwner != NULL);

	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	if (mShouldCalculateNeighborhood) {
		this->calculateNeighborhood();
	}

	// Find the average position for delegated behaviors
	//Vector2D averagePosition = ZERO_VECTOR2D;
	Vector2D averageAcc = 0;

	int neighborCount = mNeighborhood.size();
	if (neighborCount == 0) {
		this->mData = data;
		return this;
	}

	Unit* pNeighbor;
	for (int i = 0; i < neighborCount; i++) {
		pNeighbor = mNeighborhood.at(i);
		//averagePosition += pNeighbor->getPositionComponent()->getPosition();
		mpSeek->setTargetLoc(pNeighbor->getPositionComponent()->getPosition());
		averageAcc += mpSeek->getSteering()->getData().acc;
	}
	//averagePosition /= (float)neighborCount;
	averageAcc /= (float)neighborCount;

	//mpSeek->setTargetLoc(averagePosition);
	//data.acc = mpSeek->getSteering()->getData().acc;
	data.acc = averageAcc;

	this->mData = data;

	return this;
}
