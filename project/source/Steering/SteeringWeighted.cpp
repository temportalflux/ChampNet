#include "Steering\SteeringWeighted.h"

#include "Units\Unit.h"

#include "Util\Game.h"
#include "Units\UnitManager.h"
#include "Data\GameData.h"

SteeringWeighted::SteeringWeighted() : Steering() {
	mSteerings = std::deque<Steering *>();
}

SteeringWeighted::SteeringWeighted(const SteeringData& data) : SteeringWeighted() {
	this->setData(data);
}

Steering* SteeringWeighted::addWeightedSteering(Steering *steering) {
	mSteerings.push_back(steering);
	steering->setWeightKey(mSteerings.size() + ""); // default key is the 1-indexed order in the list
	return steering;
}

void SteeringWeighted::setData(const SteeringData& data) {
	Steering::setData(data);

	SteeringData subData = SteeringData(data);

	// Update steerings
	for (Steering* steering : this->mSteerings) {
		subData.type = steering->getType();
		steering->setData(subData);
	}

}

SteeringWeighted::~SteeringWeighted() {
	// Delete all steerings
	Steering* steering = NULL;
	while (this->mSteerings.size() > 0) {
		steering = this->mSteerings.front();
		this->mSteerings.pop_front();
		delete steering;
	}
}

void SteeringWeighted::getWeightData(DataPoint *dataRoot, DataPoint *&out) {
	out = dataRoot;
}

void SteeringWeighted::onDataChanged() {
	DataPoint main;
	if (gpGame->getData()->getJson(main)) {
		DataPoint *weights = NULL;
		this->getWeightData(&main, weights);
		DataPoint weightObj = *weights;

		// Load steering weights
		for (Steering* steering : mSteerings) {
			steering->setWeight((float)weightObj[steering->getWeightKey() + "-weight"].asDouble());
		}
		// Load other data from the dataroot
		this->loadMiscData(&weightObj);

	}
}

void SteeringWeighted::loadMiscData(DataPoint *weightRoot) {
	// STUB: unnecessary
}

Steering* SteeringWeighted::getSteering()
{
	Unit* pOwner = this->getUnit();
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	// Weighted Blending
	this->mData = PhysicsData();
	for (Steering* steering : mSteerings) {
		this->mData += steering->getSteering()->scaleDataWeight()->getData();
	}
	this->mData.vel = data.vel;
	this->mData.rotVel = data.rotVel;

	return this;
}
