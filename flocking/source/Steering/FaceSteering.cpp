#include "Steering\FaceSteering.h"

#include "Steering\Steering.h"
#include "Units\Unit.h"

#define _USE_MATH_DEFINES
#include <math.h>

FaceSteering::FaceSteering() : AlignSteering() {}

FaceSteering::FaceSteering(const SteeringData& data) : FaceSteering()
{
	setData(data);
}

float FaceSteering::getOrientationTarget(Unit* pOwner)
{
	// Workout the direction to the target
	Vector2D direction = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	// Check for a zero direction, and make no change if so
	if (direction.getLength() != 0) {
		// Put the target 
		// Delegate to align
		return atan2(-direction.getX(), direction.getY());
	}
	else {
		return AlignSteering::getOrientationTarget(pOwner);
	}
}

Steering* FaceSteering::getSteering()
{
	return AlignSteering::getSteering();
}
