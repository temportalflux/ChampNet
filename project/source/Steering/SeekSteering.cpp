#include <cassert>

#include "Steering\Steering.h"
#include "Steering\SeekSteering.h"
#include "Util\Game.h"
#include "Units\UnitManager.h"
#include "Units\Unit.h"

SeekSteering::SeekSteering(bool shouldFlee /*= false*/) : Steering()
{
	mShouldFlee = shouldFlee;
}

SeekSteering::SeekSteering(const SteeringData& data, bool shouldFlee /*= false*/) : SeekSteering(shouldFlee)
{
	setData(data);
}

Steering* SeekSteering::getSteering()
{

	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	Vector2D diff;

	// Get the direction to the target
	// Seek or Flee
	if (!mShouldFlee)
	{
		diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	}
	else
	{
		diff = pOwner->getPositionComponent()->getPosition() - mTargetLoc;
	}

	// Normalize to distance of 1 unit
	diff.normalize();
	diff *= pOwner->getMaxAcc();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	data.acc = diff;
	//data.rotVel = 1.0f;
	this->mData = data;
	return this;
}

