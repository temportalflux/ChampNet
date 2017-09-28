#pragma once

/*Steering - implementation of Steering class from "Artificial Intelligence for Games" - Millington and Funge
*/

#include <Vector2D.h>
#include <Trackable.h>
#include "Util\Defines.h"
#include "Steering\SteeringDefines.h"
#include "Components\PhysicsComponent.h"
#include <vector>

class Unit;
struct DataPoint;

class Steering: public Trackable
{
public:

	//constructors and destructors
	Steering( SteeringType type = INVALID_TYPE ):mType(type), mTargetLoc(ZERO_VECTOR2D), mTargetID(INVALID_UNIT_ID), mOwnerID(INVALID_UNIT_ID), mData(ZERO_PHYSICS_DATA), mWeight(1.0f) {};
	Steering(const SteeringData& data) : Steering() { setData(data); };
	virtual ~Steering(){};

	//accessors
	void setTargetLoc(const Vector2D& loc) { mTargetLoc = loc; };
	const Vector2D& getTargetLoc() const { return mTargetLoc; };
	void setTargetID(const UnitID& id) { mTargetID = id; };
	const UnitID& getTargetID() const { return mTargetID; };
	void setOwnerID(const UnitID& id) { mOwnerID = id; };
	//void setData(const PhysicsData& data) { mData = data; };
	const PhysicsData& getData() const { return mData; };
	const SteeringType getType() const { return mType; }
	virtual void setData(const SteeringData &data);

	virtual Steering* getSteering() { return this; };//overridden by sub-classes

	void update();
	virtual void onDataChanged();

	// Possible neighborhood behavior
	virtual void setDoCalculateNeighborhood(bool calculate) {}
	virtual void calculateNeighborhood(std::vector<Unit*>* filterList = NULL) {}
	virtual void setRadius(float radius) {}
	
	// Handle weighted blending for this steering
	Steering* setWeightKey(const std::string &key);
	const std::string& getWeightKey();
	void setWeight(float weight);
	Steering* scaleDataWeight();

protected:
	SteeringType mType;
	Vector2D mTargetLoc;
	UnitID mTargetID;
	UnitID mOwnerID;
	PhysicsData mData;

	Unit* getUnit();
	Unit* getUnitTarget();
	virtual bool checkTargetID();

private:
	float mWeight;
	std::string mWeightKey;

};
