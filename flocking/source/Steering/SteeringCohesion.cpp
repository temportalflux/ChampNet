#include "Steering\SteeringCohesion.h"

#include "Units\Unit.h"
#include "Steering\SeekSteering.h"

#include "Util\Game.h"
#include "Units\UnitManager.h"
#include <cassert>

using std::vector;

SteeringCohesion::SteeringCohesion() : SteeringNeighborhood() {
	mpSeek = new SeekSteering(false);
	mShouldCalculateNeighborhood = true;
}

SteeringCohesion::SteeringCohesion(const SteeringData& data) : SteeringCohesion() {
	this->setData(data);
}

void SteeringCohesion::setData(const SteeringData& data) {
	SteeringNeighborhood::setData(data);

	SteeringData subData = SteeringData(data);

	// Seek
	subData.type = this->mpSeek->getType();
	this->mpSeek->setData(subData);

}

SteeringCohesion::~SteeringCohesion() {
	delete mpSeek;
}

Steering* SteeringCohesion::getSteering()
{
	Unit* pOwner = this->getUnit();
	assert(pOwner != NULL);

	if (mShouldCalculateNeighborhood) {
		this->calculateNeighborhood();
	}

	// Find the average position for delegated behaviors
	Vector2D averagePosition = ZERO_VECTOR2D;

	int neighborCount = mNeighborhood.size();
	if (neighborCount == 0) {
		this->mData = pOwner->getPhysicsComponent()->getData();
		return this;
	}

	Unit* pNeighbor;
	for (int i = 0; i < neighborCount; i++) {
		pNeighbor = mNeighborhood.at(i);
		averagePosition += pNeighbor->getPositionComponent()->getPosition();
	}
	averagePosition /= (float)neighborCount;

	mpSeek->setTargetLoc(averagePosition);
	this->mData = mpSeek->getSteering()->getData();

	return this;
}
