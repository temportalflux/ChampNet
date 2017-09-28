#pragma once

enum SteeringType
{
	INVALID_TYPE = -1,
	// AI Behaviors
	SEEK,
	FLEE,
	ARRIVE,
	ALIGN,
	FACE,
	PURSUE,
	WANDER,
	ARRIVE_FACE,
	WANDER_SEEK,
	// Custom Behaviors
	ENEMY_FIND, // uses SEEK on target unit, on target unit destroyed, find new unit

	COHESION,
	SEPARATION,
	GROUP_ALIGNMENT,
	FLOCKING,

};

/*
struct SteeringData : public Trackable
{
	// the invalid type
	SteeringData() :
		type(SteeringType::INVALID_TYPE),
		targetLoc(ZERO_VECTOR2D),
		ownerID(INVALID_UNIT_ID),
		targetID(INVALID_UNIT_ID),
		radiusArrived(0),
		radiusArriving(0)
	{};

	// General constructor
	SteeringData(
		SteeringType theType,
		const Vector2D& theTargetLoc = ZERO_VECTOR2D,
		UnitID theOwner = INVALID_UNIT_ID, UnitID theTarget = INVALID_UNIT_ID
	) :
		type(theType),
		ownerID(theOwner),
		targetLoc(theTargetLoc),
		targetID(theTarget)
	{};

	// Constructor: Arrive
	SteeringData(
		const float radiusArrived,
		const float radiusArriving,
		const Vector2D& theTargetLoc = ZERO_VECTOR2D,
		UnitID theOwner = INVALID_UNIT_ID,
		UnitID theTarget = INVALID_UNIT_ID
	) :
		type(SteeringType::ARRIVE),
		ownerID(theOwner),
		targetLoc(theTargetLoc),
		targetID(theTarget),
		radiusArrived(radiusArrived),
		radiusArriving(radiusArrived)
	{};

	SteeringType type;
	Vector2D targetLoc;
	UnitID ownerID;
	UnitID targetID;

	float radiusArrived;
	float radiusArriving;
	float distanceWander;
	float radiusWanderSeek;

};
//*/

struct SteeringData : public Trackable {
	SteeringType type;
	UnitID ownerID;
	Vector2D targetLoc;
	UnitID targetID;

	float radiusArrived;
	float radiusArriving;
	float distanceWander;
	float radiusWanderSeek;
};
