#include <stdio.h>

#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <crtdbg.h>

#include "Util\Game.h"
#include "Graphics\GraphicsSystem.h"
#include "Graphics\GraphicsBuffer.h"
#include "Graphics\Sprite.h"
#include "Units\Unit.h"
#include "Timer.h"
#include "PerformanceTracker.h"
#include "MemoryTracker.h"
//#include "NotificationQueue.h"
#include "CircularQueue.h"
#include "Components\ComponentManager.h"
#include "Components\PositionComponent.h"
#include "Components\PhysicsComponent.h"

using namespace std;

//test callback
/*void testCallback(Notification* pMessage)
{
	cout << "message callback: message id=" << pMessage->getID() << endl;
}*/

PerformanceTracker* gpPerformanceTracker = NULL;

int main(int argc, char **argv)
{
	gpPerformanceTracker = new PerformanceTracker;
	gpPerformanceTracker->startTracking("init");

	//create the global game object
	gpGame = new Game;
	//init the game
	bool goodGame = gpGame->init();
	if (!goodGame)
	{
		fprintf(stderr, "failed to initialize Game object!\n");
		return -1;
	}

	gpPerformanceTracker->stopTracking("init");
	cout << "initialization took:" << gpPerformanceTracker->getElapsedTime("init") << "ms\n";

	bool shouldExit = false;

	//game loop
	while (!shouldExit)
	{
		gpPerformanceTracker->clearTracker("loop");
		gpPerformanceTracker->startTracking("loop");

		gpGame->beginLoop();

		gpPerformanceTracker->clearTracker("draw");
		gpPerformanceTracker->startTracking("draw");

		gpGame->processLoop();

		gpPerformanceTracker->stopTracking("draw");

		shouldExit = gpGame->endLoop();

		gpPerformanceTracker->stopTracking("loop");
		//cout << "loop took:" << gpPerformanceTracker->getElapsedTime("loop") << "ms";
		//cout << "draw took:" << gpPerformanceTracker->getElapsedTime("draw") << "ms\n";

	}
	/*{
		CircularQueue<Unit*, 10> aQueue;
		MemoryPool testPool(10, sizeof(Unit));
		Unit* ptr;

		for (Uint32 i = 0; i < 10; i++)
		{
			aQueue.pushBack(new(testPool.allocateObject()) Unit);
		}
		for (Uint32 i = 0; i < 10; i++)
		{
			aQueue.popFront(ptr);
			testPool.freeObject((Byte*)ptr);
		}

	}*/

	/*{
		ComponentManager manager(100); 
		ComponentID positionID = manager.allocatePositionComponent();
		PositionComponent* pPosComponent = manager.getPositionComponent(positionID);
		ComponentID physicsID = manager.allocatePhysicsComponent(positionID);
		PhysicsData data(Vector2D(1, 1), Vector2D(1, 1), 0.5f, 0.01f);
		PhysicsComponent* pPhysicsComponent = manager.getPhysicsComponent(physicsID);
		pPhysicsComponent->setData(data);
		for (int i = 0; i < 10; i++)
		{
			manager.update(0.1f);
		}
	}*/
	/*NotificationQueue que(10, testCallback);
	for (int i = 0; i < 10; i++)
	{
	Notification* pMessage = new Notification;
	que.doCallback(pMessage);
	delete pMessage;
	}*/

	/*CircularQueue<int> que(5);
	for (int i = 0; i < 10; i++)
	{
		que.pushBack(i);
	}

	int temp;
	que.popFront(temp);

	for (int i = 25; i < 27; i++)
	{
		que.pushBack(i);
	}

	while (que.popFront(temp))
	{
		cout << temp << ' ';
	}
	cout << endl;*/
	
	
	//system("pause");
	/*gpPerformanceTracker = new PerformanceTracker();

	gpPerformanceTracker->startTracking("init");

	//create the global game object
	gpGame = new Game;
	//init the game
	bool goodGame = gpGame->init();
	if(!goodGame) 
	{
		fprintf(stderr, "failed to initialize Game object!\n");
		return -1;
	}

	gpPerformanceTracker->stopTracking("init");
	cout << "initialization took:" << gpPerformanceTracker->getElapsedTime("init") << "ms\n";


	*/
	gpGame->cleanup();
	delete gpGame;
	gpGame = NULL;

	delete gpPerformanceTracker;
	gMemoryTracker.reportAllocations( cout );
	system("pause");

	return 0;
}