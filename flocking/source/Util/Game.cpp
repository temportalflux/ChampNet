#include <stdio.h>
#include <assert.h>

#include <sstream>

#include "Util\Game.h"
#include "Graphics\GraphicsSystem.h"
#include "Graphics\GraphicsBuffer.h"
#include "Graphics\Font.h"
#include "Graphics\GraphicsBufferManager.h"
#include "Messaging\GameMessageManager.h"
#include "Graphics\Sprite.h"
#include "Graphics\SpriteManager.h"
#include "Timer.h"
#include "Components\ComponentManager.h"
#include "Units\UnitManager.h"
#include "Input\InputSystem.h"
#include "Messaging\PlayerMoveToMessage.h"
#include "Data\GameData.h"
#include "Graphics\Audio.h"

#include <cstdlib> // random
#include <time.h> // time

Game* gpGame = NULL;

const int WIDTH = 1024;
const int HEIGHT = 768;

Game::Game()
	:mpGraphicsSystem(NULL)
	,mpGraphicsBufferManager(NULL)
	,mpSpriteManager(NULL)
	,mpLoopTimer(NULL)
	,mpMasterTimer(NULL)
	,mpFont(NULL)
	,mShouldExit(false)
	,mBackgroundBufferID("")
	,mpMessageManager(NULL)
	,mpComponentManager(NULL)
	,mpUnitManager(NULL)
	,mpInputManager(NULL)
{
	srand((unsigned int)time(NULL)); // RANDOM: Seed
}

Game::~Game()
{
	cleanup();
}

bool Game::init()
{
	mpData = NULL;

	this->loadData();

	if (!this->initVars()) {
		return false;
	}
	if (!this->initGraphics()) {
		return false;
	}

	return true;
}

bool Game::initVars() {
	mShouldExit = false;

	//create Timers
	mpLoopTimer = new Timer;
	mpMasterTimer = new Timer;

	// init input system
	mpInputManager = new InputSystem();

	//create and init GraphicsSystem
	mpGraphicsSystem = new GraphicsSystem();
	bool goodGraphics = mpGraphicsSystem->init(WIDTH, HEIGHT);
	if (!goodGraphics)
	{
		fprintf(stderr, "failed to initialize GraphicsSystem object!\n");
		return false;
	}

	mpGraphicsBufferManager = new GraphicsBufferManager(mpGraphicsSystem);
	mpSpriteManager = new SpriteManager();

	DataPoint gameData;
	int maxUnits = 500;
	if (this->getData()->getJson(gameData)) {
		maxUnits = gameData["maxUnits"].asInt();
	}

	mpMessageManager = new GameMessageManager();
	mpComponentManager = new ComponentManager(maxUnits);
	mpUnitManager = new UnitManager(maxUnits);

	mpMusic = new AudioClip("JustKeepSwimming.wav");

	return true;
}

bool Game::initGraphics() {
	//load buffers
	mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID, "wallpaper.bmp");
	mpGraphicsBufferManager->loadBuffer(mPlayerIconBufferID, "arrow2.png");
	mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferID, "enemy-arrow2_16.png");
	mpGraphicsBufferManager->loadBuffer(mTargetBufferID, "target.png");

	//load Font
	mpFont = new Font("cour.ttf", 24);

	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer(mBackgroundBufferID);
	if (pBackGroundBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight());
	}
	GraphicsBuffer* pPlayerBuffer = mpGraphicsBufferManager->getBuffer(mPlayerIconBufferID);
	Sprite* pArrowSprite = NULL;
	if (pPlayerBuffer != NULL)
	{
		pArrowSprite = mpSpriteManager->createAndManageSprite(PLAYER_ICON_SPRITE_ID, pPlayerBuffer, 0, 0, (float)pPlayerBuffer->getWidth(), (float)pPlayerBuffer->getHeight());
	}
	GraphicsBuffer* pAIBuffer = mpGraphicsBufferManager->getBuffer(mEnemyIconBufferID);
	Sprite* pEnemyArrow = NULL;
	if (pAIBuffer != NULL)
	{
		pEnemyArrow = mpSpriteManager->createAndManageSprite(AI_ICON_SPRITE_ID, pAIBuffer, 0, 0, (float)pAIBuffer->getWidth(), (float)pAIBuffer->getHeight());
	}

	GraphicsBuffer* pTargetBuffer = mpGraphicsBufferManager->getBuffer(mTargetBufferID);
	if (pTargetBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(TARGET_SPRITE_ID, pTargetBuffer, 0, 0, (float)pTargetBuffer->getWidth(), (float)pTargetBuffer->getHeight());
	}

	return true;
}

void Game::loadData() {
	if (mpData == NULL) {
		mpData = new GameData("data/data.json");
	}
	if (mpData != NULL) {
		mpData->load();
	}
}

GameData* Game::getData() {
	return mpData;
}

AudioClip* Game::getMusic() {
	return mpMusic;
}

void Game::cleanup()
{
	//delete the timers
	delete mpLoopTimer;
	mpLoopTimer = NULL;
	delete mpMasterTimer;
	mpMasterTimer = NULL;

	delete mpFont;
	mpFont = NULL;

	//delete the graphics system
	delete mpGraphicsSystem;
	mpGraphicsSystem = NULL;

	delete mpGraphicsBufferManager;
	mpGraphicsBufferManager = NULL;
	delete mpSpriteManager;
	mpSpriteManager = NULL;
	delete mpMessageManager;
	mpMessageManager = NULL;
	delete mpUnitManager;
	mpUnitManager = NULL;
	delete mpComponentManager;
	mpComponentManager = NULL;
	delete mpInputManager;
	mpInputManager = NULL;

	if (mpData != NULL) {
		delete mpData;
		mpData = NULL;
	}

	delete mpMusic;
	mpMusic = NULL;

}

void Game::beginLoop()
{
	mpMusic->play();
	mpLoopTimer->start();
}

const float TARGET_ELAPSED_MS = LOOP_TARGET_TIME / 1000.0f;
	
void Game::processLoop()
{

	// Get input
	mpInputManager->process();
	// Handle input messages
	mpMessageManager->processMessagesForThisframe();

	// Update units
	mpUnitManager->updateAll(TARGET_ELAPSED_MS);
	mpComponentManager->update(TARGET_ELAPSED_MS);
	
	//draw background
	Sprite* pBackgroundSprite = mpSpriteManager->getSprite( BACKGROUND_SPRITE_ID );
	GraphicsBuffer* pDest = mpGraphicsSystem->getBackBuffer();
	mpGraphicsSystem->draw(*pDest, *pBackgroundSprite, 0.0f, 0.0f);

	//draw units
	mpUnitManager->drawAll();

	int x = mpInputManager->getMouseX();
	int y = mpInputManager->getMouseY();

	//create mouse text
	//std::stringstream mousePos;
	//mousePos << x << ":" << y;

	//write text at mouse position
	//mpGraphicsSystem->writeText(*mpFont, (float)x, (float)y, mousePos.str(), BLACK_COLOR);

	// Render display
	mpGraphicsSystem->swap();

	// Process all messages (those which occured in this loop after input)
	mpMessageManager->processMessagesForThisframe();

	/*
	Unit* pUnit = mpUnitManager->createRandomUnit(*mpSpriteManager->getSprite(AI_ICON_SPRITE_ID));
	if (pUnit == NULL)
	{
		mpUnitManager->deleteRandomUnit();
	}
	//*/

}

bool Game::endLoop()
{
	//mpMasterTimer->start();
	mpLoopTimer->sleepUntilElapsed( LOOP_TARGET_TIME );
	return mShouldExit;
}

float genRandomBinomial()
{
	return genRandomFloat() - genRandomFloat();
}

float genRandomFloat()
{
	float r = (float)rand()/(float)RAND_MAX;
	return r;
}

Unit* Game::spawnEnemy(Sprite* sprite) {
	PositionData position = PositionData(Vector2D(
		rand() % gpGame->getGraphicsSystem()->getWidth(),
		rand() % gpGame->getGraphicsSystem()->getHeight()
	), 0.0f);

	return mpUnitManager->createUnit(*sprite, true, position);
}

