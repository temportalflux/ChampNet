#include "Messaging\MessageLoadData.h"

#include "Util\Defines.h"
#include "Util\Game.h"
#include "Units\UnitManager.h"
#include "Units\Unit.h"
#include "Data\GameData.h"

void MessageLoadData::process()
{
	gpGame->loadData();
	
	UnitManager* mpUnits = gpGame->getUnitManager();
	Vector2D idRange = mpUnits->getIDRange();
	Unit* pUnit;
	for (UnitID id = (UnitID)idRange.getX(); id < idRange.getY(); id++) {
		pUnit = mpUnits->getUnit(id);
		if (pUnit != NULL) {
			pUnit->onDataChanged();
		}
	}

	std::cout << "Loaded data\n";
	std::cout << gpGame->getData()->dump("", NULL) << '\n';

}
