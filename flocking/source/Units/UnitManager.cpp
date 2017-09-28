#include <map>

#include <Vector2D.h>

#include "Units\UnitManager.h"
#include "Units\Unit.h"
#include "Util\Game.h"
#include "Components\ComponentManager.h"
#include "Graphics\GraphicsSystem.h"

UnitID UnitManager::msNextUnitID = PLAYER_UNIT_ID;

using namespace std;

UnitManager::UnitManager(Uint32 maxSize)
	:mPool(maxSize, sizeof(Unit))
{
}

Unit* UnitManager::createUnit(const Sprite& sprite, bool shouldWrap, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/, const UnitID& id /*= INVALID_UNIT_ID*/)
{
	Unit* pUnit = NULL;

	Byte* ptr = mPool.allocateObject();
	if (ptr != NULL)
	{
		//create unit
		pUnit = new (ptr)Unit(sprite);//placement new

		UnitID theID = id;
		if (theID == INVALID_UNIT_ID)
		{
			theID = msNextUnitID;
			msNextUnitID++;
		}

		//place in map
		mUnitMap[theID] = pUnit;

		//assign id and increment nextID counter
		pUnit->mID = theID;

		//create some components
		ComponentManager* pComponentManager = gpGame->getComponentManager();
		ComponentID id = pComponentManager->allocatePositionComponent(posData,shouldWrap);
		pUnit->mPositionComponentID = id;
		pUnit->mpPositionComponent = pComponentManager->getPositionComponent(id);
		pUnit->mPhysicsComponentID = pComponentManager->allocatePhysicsComponent(pUnit->mPositionComponentID, physicsData);
		pUnit->mpComponentPhysics = pComponentManager->getPhysicsComponent(pUnit->mPhysicsComponentID);
		pUnit->mSteeringComponentID = pComponentManager->allocateSteeringComponent(pUnit->mPhysicsComponentID);
		pUnit->mpComponentSteering = pComponentManager->getSteeringComponent(pUnit->mSteeringComponentID);

		//set max's
		pUnit->mMaxSpeed = MAX_SPEED;
		pUnit->mMaxAcc = MAX_ACC;
		pUnit->mMaxRotAcc = MAX_ROT_ACC;
		pUnit->mMaxRotVel = MAX_ROT_VEL;

	}

	return pUnit;
}


Unit* UnitManager::createPlayerUnit(const Sprite& sprite, bool shouldWrap /*= true*/, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/)
{
	return createUnit(sprite, shouldWrap, posData, physicsData, PLAYER_UNIT_ID);
}


Unit* UnitManager::createRandomUnit(const Sprite& sprite)
{

	int posX = rand() % gpGame->getGraphicsSystem()->getWidth();
	int posY = rand() % gpGame->getGraphicsSystem()->getHeight();
	int velX = rand() % 50 - 25;
	int velY = rand() % 40 - 20;
	Unit* pUnit = createUnit(sprite, false, PositionData(Vector2D(posX,posY),0), PhysicsData(Vector2D(velX,velY),Vector2D(0.1f,0.1f), 0.1f, 0.05f));
	if (pUnit != NULL)
	{
		//pUnit->setSteering(Steering::SEEK, Vector2D(rand() % gpGame->getGraphicsSystem()->getWidth(), rand() % gpGame->getGraphicsSystem()->getHeight()));
		pUnit->setSteering(SteeringType::SEEK, Vector2D(gpGame->getGraphicsSystem()->getWidth()/2, gpGame->getGraphicsSystem()->getHeight()/2));
	}
	return pUnit;
}

void UnitManager::getRandomUnit(UnitID& out, bool allowFirst) const {
	if (mUnitMap.size() >= 1)
	{
		Uint32 target = rand() % mUnitMap.size();
		if (!allowFirst && target == 0)//don't allow the 0th element to be deleted as it is the player unit
		{
			target = 1;
		}

		Uint32 cnt = 0;
		for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it, cnt++)
		{
			if (cnt == target)
			{
				out = it->first;
				break;
			}
		}
		// NOTE: This does not account for IDs which go unused when the unit gets deleted
	}
}

Unit* UnitManager::getUnit(const UnitID& id) const
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void UnitManager::deleteUnit(const UnitID& id)
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		Unit* pUnit = it->second;//hold for later

		//remove from map
		mUnitMap.erase(it);

		//remove components
		ComponentManager* pComponentManager = gpGame->getComponentManager();
		pComponentManager->deallocatePhysicsComponent(pUnit->mPhysicsComponentID);
		pComponentManager->deallocatePositionComponent(pUnit->mPositionComponentID);
		pComponentManager->deallocateSteeringComponent(pUnit->mSteeringComponentID);

		//call destructor
		pUnit->~Unit();

		//free the object in the pool
		mPool.freeObject((Byte*)pUnit);
	}
}

void UnitManager::deleteRandomUnit(unsigned int quantity, bool allowFirst)
{
	UnitID id;
	//std::vector<UnitID> ids = std::vector<UnitID>();
	for (unsigned int i = 0; i < quantity; i++) {
		this->getRandomUnit(id, allowFirst);
		//ids.push_back(id);
		deleteUnit(id);
	}
	
}

void UnitManager::drawAll() const
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->draw();
	}
}

void UnitManager::updateAll(float elapsedTime)
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->update(elapsedTime);
	}
}

Vector2D UnitManager::getIDRange() const {
	return Vector2D((int)PLAYER_UNIT_ID, (int)msNextUnitID - 1);
}

std::vector<Unit*> UnitManager::units() const {
	vector<Unit*> units = vector<Unit*>();

	UnitManager* mpUnits = gpGame->getUnitManager();
	Vector2D idRange = mpUnits->getIDRange();
	Unit* pUnit;

	for (UnitID id = (UnitID)idRange.getX(); id <= idRange.getY(); id++) {
		pUnit = mpUnits->getUnit(id);
		if (pUnit != NULL) {
			units.push_back(pUnit);
		}
	}

	return units;
}
