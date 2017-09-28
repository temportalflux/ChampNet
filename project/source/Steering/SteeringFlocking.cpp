#include "Steering\SteeringFlocking.h"

#include "Units\Unit.h"
#include "Steering\SteeringNeighborhood.h"
#include "Steering\AlignSteering.h"
#include "Steering\SteeringGroupAlignment.h"
#include "Steering\SteeringCohesion.h"
#include "Steering\SteeringSeparation.h"

#include "Util\Game.h"
#include "Units\UnitManager.h"
#include "Data\GameData.h"

using std::vector;

SteeringFlocking::SteeringFlocking() : SteeringWeighted() {
	mpNeighborhoodTracker = new SteeringNeighborhood();
	mpNeighborhoodTracker->setDoCalculateNeighborhood(true);
	mpFace = new AlignSteering();

	addWeightedSteering(new SteeringGroupAlignment())
		->setWeightKey("alignment")
		->setDoCalculateNeighborhood(false);
	addWeightedSteering(new SteeringCohesion())
		->setWeightKey("cohesion")
		->setDoCalculateNeighborhood(false);
	addWeightedSteering(new SteeringSeparation())
		->setWeightKey("separation")
		->setDoCalculateNeighborhood(false);
	
	this->onDataChanged();
}

SteeringFlocking::SteeringFlocking(const SteeringData& data) : SteeringFlocking() {
	this->setData(data);
}

SteeringFlocking::~SteeringFlocking() {
	delete mpNeighborhoodTracker;
	delete mpFace;
}

void SteeringFlocking::setData(const SteeringData& data) {
	SteeringWeighted::setData(data);

	SteeringData subData = SteeringData(data);

	// Neightborhood
	subData.type = mpNeighborhoodTracker->getType();
	mpNeighborhoodTracker->setData(subData);

	// Face
	subData.type = mpFace->getType();
	mpFace->setData(subData);

}

Steering* SteeringFlocking::getSteering()
{
	Unit* pOwner = this->getUnit();
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	// Calculate the neighborhood based on the MAX radius of all three behaviors (see onDataChanged)
	mpNeighborhoodTracker->calculateNeighborhood();
	vector<Unit*> neighborhoodMax = mpNeighborhoodTracker->mNeighborhood;

	for (Steering* steering : this->mSteerings) {
		steering->calculateNeighborhood(&neighborhoodMax);
	}

	this->mData = SteeringWeighted::getSteering()->getData();
	this->mData.rotAcc = mpFace->getSteering()->getData().rotAcc;

	return this;
}

void SteeringFlocking::getWeightData(DataPoint *dataRoot, DataPoint *&out) {
	// TODO: returning address of local variable or temporary
	out = &((*dataRoot)["flocking"]);
}

void SteeringFlocking::loadMiscData(DataPoint *weightRoot) {
	DataPoint weightObj = *weightRoot;
	float radiusMax = 0;
	float radius;
	// Load radus
	for (Steering* steering : mSteerings) {
		radius = (float)weightObj[steering->getWeightKey() + "-radius"].asDouble();
		steering->setRadius(radius);
		radiusMax = max(radiusMax, radius);
	}
	this->mpNeighborhoodTracker->setRadius(radiusMax);
}
