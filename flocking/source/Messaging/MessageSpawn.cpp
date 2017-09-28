#include "Messaging\MessageSpawn.h"

#include "Util\Game.h"
#include "Data\GameData.h"
#include "Units\UnitManager.h"
#include "Graphics\SpriteManager.h"

MessageSpawn::MessageSpawn()
	:GameMessage(ENEMY_SPAWN)
{
}

MessageSpawn::~MessageSpawn()
{
}

void MessageSpawn::process()
{
	DataPoint gameData;
	int quantity = 10;
	float velocityMin = 0.15f, velocityMax = 0.35f;
	bool randomAsBatch = true;
	if (gpGame->getData()->getJson(gameData)) {
		quantity = gameData["controls"]["spawnAmount"].asInt();
		velocityMin = (float)gameData["spawnVelocity"]["min"].asDouble();
		velocityMax = (float)gameData["spawnVelocity"]["max"].asDouble();
		randomAsBatch = gameData["spawnVelocity"]["randomInBatch"].asBool();
	}

	SteeringData data;
	data.type = SteeringType::FLOCKING;
	// Used for Cohesion
	data.radiusArrived = 10;
	data.radiusArriving = 100;
	data.targetLoc = ZERO_VECTOR2D;

	PhysicsData physics;

	float maxSpeed = MAX_SPEED;// sqrt(MAX_SPEED);

	// the base velocity (and amount in between the min and max)
	// ex: velMax = 0.35, velMin = 0.15 => 0.2
	// ex: velMax = 0.5, velMin = 0.1 => 0.4
	// ex: velMax = -0.2, velMin = 0.3 => 0.5
	float velocityRange = velocityMax - velocityMin;

	if (randomAsBatch) {
		// Set random velocity for the BATCH
		physics.vel = Vector2D(
			// ex: velocityRange = 0.2, velocityMin = 0.15 => 0.15 + [0, 0.2) => random in range [0.15, 0.35)
			// ex: velocityRange = 0.4, velocityMin = 0.10 => 0.10 + [0, 0.4) => random in range [0.10, 0.50)
			// ex: velocityRange = 0.5, velocityMin = -0.2 => -0.2 + [0, 0.5) => random in range [-0.2, 0.30)
			(velocityMin + (((float)rand() / (float)RAND_MAX) * velocityRange)) * maxSpeed,
			(velocityMin + (((float)rand() / (float)RAND_MAX) * velocityRange)) * maxSpeed
		);
	}

	Sprite* icon = gpGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID);
	Unit* pUnit;
	for (int i = 0; i < quantity; i++) {
		pUnit = gpGame->spawnEnemy(icon);
		if (pUnit != NULL) {
			pUnit->setShowTarget(false);
			data.ownerID = pUnit->getID();
			// Setup Steering
			pUnit->setSteering(data);

			// Set random facingA
			pUnit->getPositionComponent()->setFacing((float)(rand() % 360));

			if (!randomAsBatch) {
				// Set random velocity for the INDIVIDUAL
				physics.vel = Vector2D(
					// ex: velocityRange = 0.2, velocityMin = 0.15 => 0.15 + [0, 0.2) => random in range [0.15, 0.35)
					// ex: velocityRange = 0.4, velocityMin = 0.10 => 0.10 + [0, 0.4) => random in range [0.10, 0.50)
					// ex: velocityRange = 0.5, velocityMin = -0.2 => -0.2 + [0, 0.5) => random in range [-0.2, 0.30)
					(velocityMin + (((float)rand() / (float)RAND_MAX) * velocityRange)) * maxSpeed,
					(velocityMin + (((float)rand() / (float)RAND_MAX) * velocityRange)) * maxSpeed
				);
			}

			pUnit->getPhysicsComponent()->setData(physics);
		}
	}

	std::cout << "Unit count: " << gpGame->getUnitManager()->units().size() << '\n';

}
