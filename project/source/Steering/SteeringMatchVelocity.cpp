#include "Steering\SteeringMatchVelocity.h"

#include "Units\Unit.h"

SteeringMatchVelocity::SteeringMatchVelocity() : Steering() {}

Steering* SteeringMatchVelocity::getSteering()
{
	Unit* pTarget = this->getUnitTarget();
	PhysicsData dataTarget = pTarget->getPhysicsComponent()->getData();
	return this->getSteeringFor(dataTarget.vel);
}

Steering* SteeringMatchVelocity::getSteeringFor(Vector2D velocity) {
	Unit* pOwner = this->getUnit();
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	//data.vel = velocity;// -data.vel;
	data.acc = velocity - data.vel;
	data.acc /= 0.5f;

	if (data.acc.getLengthSquared() > data.maxAccMagnitude * data.maxAccMagnitude) {
		data.acc.normalize();
		data.acc *= data.maxAccMagnitude;
	}

	this->mData = data;

	return this;
}
