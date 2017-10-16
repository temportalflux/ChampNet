#include "Game.h"

Game::Game()
{
	mIsRunning = false;
}

Game::~Game()
{

}

void Game::update()
{
	this->updateInput();
	this->updateNetwork();
	this->updateGame();
	this->render();
}

void Game::updateInput()
{

}

void Game::updateNetwork()
{

}

void Game::updateGame()
{

}

void Game::render()
{

}
