#include "Units\Unit.h"
#include <assert.h>

#include "Util\Game.h"
#include "Graphics\GraphicsSystem.h"
#include "Components\Component.h"
#include "Components\PositionComponent.h"
#include "Components\PhysicsComponent.h"
#include "Components\SteeringComponent.h"
#include "Components\ComponentManager.h"
#include "Graphics\SpriteManager.h"


Unit::Unit(const Sprite& sprite) 
	:mSprite(sprite)
	,mPositionComponentID(INVALID_COMPONENT_ID)
	,mPhysicsComponentID(INVALID_COMPONENT_ID)
	,mSteeringComponentID(INVALID_COMPONENT_ID)
	,mShowTarget(false)
{
}

Unit::~Unit()
{
}

void Unit::draw() const
{
	PositionComponent* pPosition = getPositionComponent();
	assert(pPosition != NULL);
	const Vector2D& pos = pPosition->getPosition();
	gpGame->getGraphicsSystem()->draw(mSprite, pos.getX(), pos.getY(), pPosition->getFacing());

	if (mShowTarget)
	{
		SteeringComponent* pSteering = getSteeringComponent();
		assert(pSteering != NULL);
		const Vector2D& targetLoc = pSteering->getTargetLoc();
		if (&targetLoc != &ZERO_VECTOR2D)
		{
			Sprite* pTargetSprite = gpGame->getSpriteManager()->getSprite(TARGET_SPRITE_ID);
			assert(pTargetSprite != NULL);
			gpGame->getGraphicsSystem()->draw(*pTargetSprite, targetLoc.getX(), targetLoc.getY());
		}
	}
}

float Unit::getFacing() const
{
	PositionComponent* pPosition = getPositionComponent();
	assert(pPosition != NULL);
	return pPosition->getFacing();
}

PositionComponent* Unit::getPositionComponent() const
{
	return mpPositionComponent;
}

PhysicsComponent* Unit::getPhysicsComponent() const
{
	//PhysicsComponent* pComponent = gpGame->getComponentManager()->getPhysicsComponent(mPhysicsComponentID);
	return mpComponentPhysics;
}

SteeringComponent* Unit::getSteeringComponent() const
{
	//SteeringComponent* pComponent = gpGame->getComponentManager()->getSteeringComponent(mSteeringComponentID);
	return mpComponentSteering;
}

void Unit::setSteering(SteeringType type, Vector2D targetLoc /*= ZERO_VECTOR2D*/, UnitID targetUnitID /*= INVALID_UNIT_ID*/)
{
	SteeringData data;
	data.type = type;
	data.targetLoc = targetLoc;
	data.targetID = targetUnitID;
	data.ownerID = mID;
	setSteering(data);
}

void Unit::setSteering(SteeringData& data) {
	SteeringComponent* pSteeringComponent = getSteeringComponent();
	if (pSteeringComponent != NULL)
	{
		pSteeringComponent->setData(data);
	}
}

void Unit::setSteeringTarget(Vector2D targetLoc /*= ZERO_VECTOR2D*/, UnitID targetUnitID /*= INVALID_UNIT_ID*/) {
	SteeringComponent* pSteeringComponent = getSteeringComponent();
	if (pSteeringComponent != NULL)
	{
		SteeringData data = pSteeringComponent->getData();
		data.targetLoc = targetLoc;
		data.targetID = targetUnitID;

		pSteeringComponent->setData(data);
	}
}

void Unit::onDataChanged() {
	SteeringComponent* pSteeringComponent = getSteeringComponent();
	if (pSteeringComponent != NULL)
	{
		pSteeringComponent->onDataChanged();
	}
}
