#include "Steering\SteeringEnemyFind.h"

#include "Util\Game.h"
#include "Units\UnitManager.h"
#include "Units\Unit.h"
#include <Vector2D.h>

SteeringEnemyFind::SteeringEnemyFind() : SeekSteering() {}

SteeringEnemyFind::SteeringEnemyFind(const SteeringData& data) : SteeringEnemyFind()
{
	setData(data);
}

Steering* SteeringEnemyFind::getSteering()
{
	/*
	if (mTargetID != INVALID_UNIT_ID)
	{
		Unit* pTarget = this->getUnitTarget();
		if (pTarget == NULL) {
			std::cout << "Finding new target\n";
			// Target has been deleted somewhere along the way
			
			for (int attempt = 1; attempt <= 5; attempt++) {

				// Find new target
				Vector2D rangeID = gpGame->getUnitManager()->getIDRange();
				// rand is seeded in Game constructor
				UnitID newID = rand() % (int)(rangeID.getY() - rangeID.getX()) + (int)rangeID.getX();

				if (gpGame->getUnitManager()->getUnit(newID) != NULL) {
					// Found a valid target ID
					mTargetID = newID;
					break;
				}
				// Not a valid ID, try below and above
				else {
					if (gpGame->getUnitManager()->getUnit(newID + 1) != NULL) {
						// Found a valid target ID
						mTargetID = newID + 1;
						break;
					}
					if (gpGame->getUnitManager()->getUnit(newID - 1) != NULL) {
						// Found a valid target ID
						mTargetID = newID - 1;
						break;
					}
				}

			}

		}
	}
	*/
	
	return SeekSteering::getSteering();
}
