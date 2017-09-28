#include "Steering\SteeringNeighborhood.h"

#include "Util\Game.h"
#include "Units\UnitManager.h"
#include "Units\Unit.h"
#include <cassert>

using std::vector;

SteeringNeighborhood::SteeringNeighborhood() : Steering() {
	mRadiusNeighborhood = 200;
	mNeighborhood = vector<Unit*>();
}

SteeringNeighborhood::SteeringNeighborhood(const SteeringData& data) : SteeringNeighborhood() {
	this->setData(data);
}

void SteeringNeighborhood::setRadius(float radius) {
	mRadiusNeighborhood = radius;
}

float SteeringNeighborhood::getRadius() {
	return mRadiusNeighborhood;
}

void SteeringNeighborhood::setDoCalculateNeighborhood(bool calculate) {
	this->mShouldCalculateNeighborhood = calculate;
}

vector<Unit*> SteeringNeighborhood::calculateNeighborhood(Unit* owner, float radius, std::vector<Unit*>* filterList) {
	float radiusSq = radius * radius;
	
	// If no filter is passed in, get a list of all units
	vector<Unit*> units;
	if (filterList != NULL) {
		units = *filterList;
	}
	else {
		units = gpGame->getUnitManager()->units();
	}

	// Find all the units that are within a radius
	vector<Unit*> neighborhood = vector<Unit*>();
	Vector2D diff;
	float lengthSq;

	for (auto pUnit : units) {
		if (pUnit == owner) continue;
		diff = owner->getPositionComponent()->getPosition() - pUnit->getPositionComponent()->getPosition();
		lengthSq = diff.getLengthSquared();
		if (lengthSq > 0 && lengthSq <= radiusSq) { // is inside the radius
			neighborhood.push_back(pUnit);
		}
	}

	return neighborhood;
}

void SteeringNeighborhood::calculateNeighborhood(std::vector<Unit*>* filterList) {
	mNeighborhood = this->calculateNeighborhood(this->getUnit(), mRadiusNeighborhood, filterList);
}
