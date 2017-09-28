#include "Steering\SteeringMatchAlignment.h"

#include "Units\Unit.h"

SteeringMatchAlignment::SteeringMatchAlignment() : Steering() {}

Steering* SteeringMatchAlignment::getSteering()
{
	Unit* pTarget = this->getUnitTarget();
	PhysicsData dataTarget = pTarget->getPhysicsComponent()->getData();
	return this->getSteeringFor(dataTarget.rotVel);
}

Steering* SteeringMatchAlignment::getSteeringFor(float facing) {
	Unit* pOwner = this->getUnit();
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	data.rotAcc = facing - pOwner->getFacing();
	data.rotAcc /= 0.1f;
	data.rotAcc /= 0.1f;

	if (data.rotAcc > data.maxRotAcc) {
		data.rotAcc = data.maxRotAcc;
	}

	this->mData = data;

	return this;
}
