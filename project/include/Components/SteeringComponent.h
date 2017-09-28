#pragma once

#include <Trackable.h>
#include <Vector2D.h>
#include "Components\Component.h"
#include "Units\Unit.h"
#include "Steering\SteeringDefines.h"

class PhysicsComponent;
class Steering;

const SteeringData ZERO_STEERING_DATA;

class SteeringComponent : public Component
{
public:
	//getters and setters
	SteeringType getType() const { return mData.type; };
	const Vector2D& getTargetLoc() const { return mData.targetLoc; };
	UnitID getOwnerID() const { return mData.ownerID; };
	UnitID getTargetID() const { return mData.targetID; };
	ComponentID getPhysicsComponentID() const { return mPhysicsComponentID; };
	const SteeringData& getData() const { return mData; };
	void setData(const SteeringData& data);
	void setTargetLoc(const Vector2D& pos) { mData.targetLoc = pos; };
	void setTargetUnitID(const UnitID& id) { mData.targetID = id; };
	void applySteering(PhysicsComponent& physicsComponent);
	void onDataChanged();
private:
	ComponentID mPhysicsComponentID;
	SteeringData mData;
	Steering* mpSteering;
	PhysicsComponent* mpPhysicsComponent=NULL;

	SteeringComponent(const ComponentID& id, const ComponentID& physicsComponentID);
	~SteeringComponent();
	void setPhysicsComponent(PhysicsComponent* pComp) { mpPhysicsComponent = pComp; };

	friend class ComponentManager;
};