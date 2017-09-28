#include "Components\ComponentManager.h"
#include "Components\SteeringComponent.h"
#include "Steering\SeekSteering.h"
#include "Steering\ArriveSteering.h"
#include "Steering\AlignSteering.h"
#include "Steering\FaceSteering.h"

#include "Steering\WanderSteering.h"
#include "Steering\ArriveAndFaceSteering.h"
#include "Steering\WanderAndChaseSteering.h"
#include "Steering\SteeringEnemyFind.h"

#include "Steering\SteeringGroupAlignment.h"
#include "Steering\SteeringCohesion.h"
#include "Steering\SteeringSeparation.h"
#include "Steering\SteeringFlocking.h"

SteeringComponent::SteeringComponent(const ComponentID& id, const ComponentID& physicsComponentID) 
	:Component(id)
	, mPhysicsComponentID(physicsComponentID)
	, mpSteering(NULL)
{
}

SteeringComponent::~SteeringComponent()
{
	delete mpSteering;
}

void SteeringComponent::applySteering(PhysicsComponent& physicsComponent)
{
	if (mpSteering != NULL)
	{
		//allow Steering to run
		mpSteering->update();
		//set physics data to that of the Steering
		physicsComponent.setData(mpSteering->getData());
		//update component's data
		mData.targetLoc = mpSteering->getTargetLoc();
	}
}

void SteeringComponent::setData(const SteeringData& data)
{
	SteeringType typePrevious = mData.type;

	mData = data;

	if (mData.type != typePrevious) {
		//cleanup old steering
		delete mpSteering;
		mpSteering = NULL;
	}

	//create new steering
	if (mpSteering != NULL) {
		mpSteering->setData(data);
	}
	else {
		switch (data.type)
		{
			case SteeringType::SEEK:
				mpSteering = new SeekSteering(data, false);
				break;
			case SteeringType::FLEE:
				mpSteering = new SeekSteering(data, true);
				break;
			case SteeringType::ARRIVE:
				mpSteering = new ArriveSteering(data);
				break;
			case SteeringType::ALIGN:
				mpSteering = new AlignSteering(data);
				break;
			case SteeringType::FACE:
				mpSteering = new FaceSteering(data);
				break;
			case SteeringType::PURSUE:
				mpSteering = new FaceSteering(data); // TODO
				break;
			case SteeringType::WANDER:
				mpSteering = new WanderSteering(data);
				break;
			case SteeringType::ARRIVE_FACE:
				mpSteering = new ArriveAndFaceSteering(data);
				break;
			case SteeringType::WANDER_SEEK:
				mpSteering = new WanderAndChaseSteering(data);
				break;
			case SteeringType::ENEMY_FIND:
				mpSteering = new SteeringEnemyFind(data);
				break;
			case SteeringType::COHESION:
				mpSteering = new SteeringCohesion(data);
				break;
			case SteeringType::SEPARATION:
				mpSteering = new SteeringSeparation(data);
				break;
			case SteeringType::GROUP_ALIGNMENT:
				mpSteering = new SteeringGroupAlignment(data);
				break;
			case SteeringType::FLOCKING:
				mpSteering = new SteeringFlocking(data);
				break;
			case SteeringType::INVALID_TYPE:
				// No behavior for this, a valid id was specified, but it is the only behavior that should be without a class
				break;
			default:
				{
					std::cout << "Warning: missing AI class for behavior enum " << data.type << '\n';
					break;
				}
		}
	}
	

}

void SteeringComponent::onDataChanged() {
	if (mpSteering != NULL) {
		mpSteering->onDataChanged();
	}
}

