#include "Steering\WanderSteering.h"

#include "Units\Unit.h"

#include "Util\Game.h"
#include "Graphics\GraphicsSystem.h"

WanderSteering::WanderSteering() : ArriveAndFaceSteering() {}

WanderSteering::WanderSteering(const SteeringData& data) : WanderSteering()
{
	setData(data);
}

void WanderSteering::setData(const SteeringData& data) {
	ArriveAndFaceSteering::setData(data);
	// targetLoc is used for wander target
	//
	mDistanceWanderSq = data.distanceWander * data.distanceWander;
}

void WanderSteering::findNextTarget(Vector2D &out) {
	int width = gpGame->getGraphicsSystem()->getWidth();
	int height = gpGame->getGraphicsSystem()->getHeight();
	out = Vector2D(
		genRandomBinomial() * (width * 0.5f) + (width * 0.5f),
		genRandomBinomial() * (height * 0.5f) + (height * 0.5f)
	);
}

Steering* WanderSteering::getSteering()
{
	// TEMPORARY: Find a better way to set the starting location to something random
	if (mTargetLoc == ZERO_VECTOR2D) {
		this->findNextTarget(mTargetLoc);
	}

	Vector2D targetDiff = mTargetLoc - this->getUnit()->getPositionComponent()->getPosition();
	float targetDistSq = targetDiff.getLengthSquared();
	
	if (targetDistSq < mDistanceWanderSq) {
		// Wander to a new target
		this->findNextTarget(mTargetLoc);
	}
	
	ArriveAndFaceSteering::getSteering();

	return this;
}
