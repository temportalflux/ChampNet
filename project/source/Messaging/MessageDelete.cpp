#include "Messaging\MessageDelete.h"

#include "Util\Game.h"
#include "Data\GameData.h"
#include "Units\UnitManager.h"

MessageDelete::MessageDelete()
	:GameMessage(ENEMY_DELETE)
{
}

MessageDelete::~MessageDelete()
{
}

void MessageDelete::process()
{
	DataPoint data;
	int amount = 1;
	if (gpGame->getData()->getJson(data)) {
		amount = data["controls"]["killAmount"].asInt();
	}
	gpGame->getUnitManager()->deleteRandomUnit(amount, true);

	std::cout << "Unit count: " << gpGame->getUnitManager()->units().size() << '\n';
}
