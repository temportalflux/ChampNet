#include "Steering\AlignSteering.h"

#include "Steering\Steering.h"
#include "Units\Unit.h"

#define _USE_MATH_DEFINES
#include <math.h>

AlignSteering::AlignSteering() : Steering() {}

AlignSteering::AlignSteering(const SteeringData& data) : AlignSteering()
{
	setData(data);
}

float AlignSteering::getOrientationTarget(Unit* pOwner) {
	//return pOwner->getPositionComponent()->getFacing();
	Vector2D position = pOwner->getPhysicsComponent()->getVelocity();
	return atan2(-position.getX(), position.getY());
}

Steering* AlignSteering::getSteering()
{

	Unit* pOwner = this->getUnit();
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	float targetOrientation = this->getOrientationTarget(pOwner);
	
	// Get the naive direction to the target
	float rotation = targetOrientation -
		pOwner->getPositionComponent()->getFacing();

	// Map the result to the (-pi, pi) interval
	// Map the rotation to the range 0-2PI
	rotation = rotation - (float)(2 * M_PI) * floor(rotation / (float)(2 * M_PI));
	// Map the rotation from [0,2PI] to [-PI,PI]
	rotation -= (float)M_PI;
	// Find the nominal distance from the target orientation
	float rotationSize = abs(rotation);

	float radiusArriving = (float)M_PI * 1.0f;
	float radiusArrived = 0.1f * (float)M_PI;

	float targetRotation = 1.0f;
	// Check if we are there,
	// then set the target rotation to the opposite of the current
	if (rotationSize < radiusArrived) {
		targetRotation = data.rotVel;
	}
	// If we are outside the slowRadius, then use maximum rotation
	else if (rotationSize > radiusArriving) {
		targetRotation = data.maxRotAcc;
	}
	// Otherwise calculate a scaled rotation 
	else {
		targetRotation = data.maxRotAcc * rotationSize / radiusArriving;
	}

	// The final target rotation combines
	// speed (already in the target rotation) and
	// direction (must normalize the rotation)
	targetRotation *= rotation / rotationSize;

	// Acceleration tries to get to the target rotation
	data.rotAcc = targetRotation - data.rotVel;
	data.rotAcc /= 0.5f;

	// Check if the acceleration is too great
	if (abs(data.rotAcc) > data.maxRotAcc) {
		data.rotAcc /= abs(data.rotAcc);
		data.rotAcc *= data.maxRotAcc;
	}

	// Output the steering
	this->mData = data;

	return this;
}
