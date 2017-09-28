#include "Steering\WanderAndChaseSteering.h"

#include "Units\Unit.h"
#include "Util\Game.h"

WanderAndChaseSteering::WanderAndChaseSteering() : WanderSteering() {}

WanderAndChaseSteering::WanderAndChaseSteering(const SteeringData &data) : WanderAndChaseSteering() {
	setData(data);
}

void WanderAndChaseSteering::setData(const SteeringData &data) {
	WanderSteering::setData(data);
	this->mRadiusSeekSq = data.radiusWanderSeek * data.radiusWanderSeek;
	this->mSteeringFace.mTargetID = INVALID_UNIT_ID;
}

Steering* WanderAndChaseSteering::getSteering()
{
	Vector2D targetDiff = ZERO_VECTOR2D;
	float targetDistSq = -1;
	
	Unit* pOwner = this->getUnit();

	// If the AI has a unit it is seeking
	if (this->mTargetID != INVALID_UNIT_ID) {
		Vector2D targetPos = this->getUnitTarget()->getPositionComponent()->getPosition();
		// find the distance to the unit
		targetDiff = targetPos - pOwner->getPositionComponent()->getPosition();
		targetDistSq = targetDiff.getLengthSquared();

		//std::cout << (targetDistSq < mRadiusSeekSq) << '\n';

		if (targetDistSq < mRadiusSeekSq) {
			// Seek the target

			// stolen from SeekSteering...

			// Normalize to distance of 1 unit
			targetDiff.normalize();
			targetDiff *= pOwner->getMaxAcc();

			PhysicsData data = pOwner->getPhysicsComponent()->getData();
			data.acc = targetDiff;

			this->mData = data;

			this->mSteeringFace.mTargetLoc = targetPos;
			this->mSteeringFace.getSteering();
			this->mData.rotAcc = this->mSteeringFace.mData.rotAcc;

			return this;
		}
	}

	// Go back to wandering
	WanderSteering::getSteering();

	return this;
}
