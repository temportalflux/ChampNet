#include <cassert>

#include "Steering\Steering.h"
#include "Steering\ArriveSteering.h"
#include "Util\Game.h"
#include "Units\UnitManager.h"
#include "Units\Unit.h"

ArriveSteering::ArriveSteering() : Steering(SteeringType::ARRIVE) {}

ArriveSteering::ArriveSteering(const SteeringData& data) : ArriveSteering()
{
	setData(data);
}

void ArriveSteering::setData(const SteeringData& data) {
	Steering::setData(data);
	mRadiusArrived = data.radiusArrived * data.radiusArrived;
	mRadiusArriving = data.radiusArriving * data.radiusArriving;
}

Steering* ArriveSteering::getSteering()
{

	Unit* pOwner = this->getUnit();
	Vector2D diff;
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	// Get the direction to the target
	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	// Find the nominal distance of the direction to the target
	float distance = diff.getLengthSquared();

	// Check if we are there,
	// then set the velocity and acceleration to nil
	if (distance < mRadiusArrived) {
		// Owner has arrived
		//data.vel *= 0;
		data.acc *= 0;
	}
	else {
		float targetSpeed;

		// If we are outside the slowRadius, then go max speed
		if (distance > mRadiusArriving) {
			// Owner should move at max speed (Seek)
			targetSpeed = pOwner->getMaxAcc();
		}
		// Otherwise calculate a scaled speed
		else {
			// Owner should slow down
			targetSpeed = pOwner->getMaxAcc() * distance / mRadiusArriving;
		}

		// The target velocity combines speed and direction
		// Normalize to distance of 1 unit
		diff.normalize();
		// Combine the target speed with the current velocity vector
		diff *= targetSpeed;

		// Acceleration trieds to get to the target velocity
		data.acc = diff - data.vel;
		data.acc /= 0.1f;

		// Check if the acceleration is too fast
		if (data.acc.getLengthSquared() > pOwner->getMaxAcc() * pOwner->getMaxAcc()) {
			data.acc.normalize();
			data.acc *= pOwner->getMaxAcc();
		}
	}

	// Output the steering
	this->mData = data;

	return this;
}

