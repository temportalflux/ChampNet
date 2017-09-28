#include "Steering\ArriveAndFaceSteering.h"

ArriveAndFaceSteering::ArriveAndFaceSteering() : ArriveSteering(), mSteeringFace() {}

ArriveAndFaceSteering::ArriveAndFaceSteering(const SteeringData& data) : ArriveAndFaceSteering()
{
	setData(data);
}

void ArriveAndFaceSteering::setData(const SteeringData& data) {
	ArriveSteering::setData(data);
	mSteeringFace.setData(data);
}

Steering* ArriveAndFaceSteering::getSteering()
{
	ArriveSteering::getSteering();
	
	mSteeringFace.mTargetLoc = mTargetLoc;

	mSteeringFace.getSteering();
	this->mData.rotAcc = mSteeringFace.mData.rotAcc;

	return this;
}