#include "Steering\Steering.h"

#include "Util\Game.h"
#include "Units\UnitManager.h"
#include "Units\Unit.h"
#include <cassert>
#include "Data\GameData.h"

using std::vector;

void Steering::update()
{
	Steering* pTemp = getSteering();
	if (pTemp != NULL)
	{
		mData = pTemp->getData();
	}
}

void Steering::setData(const SteeringData &data) {
	mType = data.type;
	mTargetLoc = data.targetLoc;
	mTargetID = data.targetID;
	mOwnerID = data.ownerID;
}

Unit* Steering::getUnit() {
	return gpGame->getUnitManager()->getUnit(mOwnerID);
}

Unit* Steering::getUnitTarget() {
	return gpGame->getUnitManager()->getUnit(mTargetID);
}

bool Steering::checkTargetID() {
	// Is the target unit valid?
	// Assumed that target location is valid if there is no valid target unit
	bool hasTarget = mTargetID != INVALID_UNIT_ID;
	if (hasTarget)
	{
		Unit* pTarget = this->getUnitTarget();
		if (pTarget != NULL) {
			// Set target location from position of target unit
			mTargetLoc = pTarget->getPositionComponent()->getPosition();
		}
		else {
			// Target has been deleted somewhere along the way
			mTargetID = INVALID_UNIT_ID;
		}
	}
	return hasTarget;
}

void Steering::onDataChanged() {
}

Steering* Steering::setWeightKey(const std::string &key) {
	this->mWeightKey = key;
	return this;
}

const std::string& Steering::getWeightKey() {
	return mWeightKey;
}

void Steering::setWeight(float weight) {
	this->mWeight = weight;
}

Steering* Steering::scaleDataWeight() {
	this->mData *= this->mWeight;
	return this;
}
