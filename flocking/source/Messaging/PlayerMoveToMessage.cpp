#include "Util\Game.h"
#include "Messaging\GameMessageManager.h"
#include "Messaging\PlayerMoveToMessage.h"
#include "Units\UnitManager.h"
#include "Components\SteeringComponent.h"

PlayerMoveToMessage::PlayerMoveToMessage( const Vector2D& pos )
:GameMessage(PLAYER_MOVETO_MESSAGE)
,mPos(pos)
{
}

PlayerMoveToMessage::~PlayerMoveToMessage()
{
}

void PlayerMoveToMessage::process()
{
	Unit* pPlayer = gpGame->getUnitManager()->getPlayerUnit();
	// Offset for cursor point
	mPos.setX(mPos.getX() - 15);
	mPos.setY(mPos.getY() - 15);
	// Set target location
	pPlayer->setSteeringTarget(mPos);
}

