#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Chapter0Level1.h"
using namespace CocosDenshion;

cocos2d::Scene * Chapter0Level1::createScene()
{
	return Chapter0Level1::create();
}

void Chapter0Level1::onEnterTransitionDidFinish()
{
	GameScene::onEnterTransitionDidFinish();
}

bool Chapter0Level1::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	initMap("map/chapter0Level1.tmx");
	initBackgroundMusic();
	initDashboard();
	initListener();

	revivePoint = hero->getPosition();
	heroDied = heroJumped = 0;

	this->schedule(schedule_selector(GameScene::heroUpdate));
	this->schedule(schedule_selector(GameScene::mapUpdate));
	this->schedule(schedule_selector(GameScene::regenerateUpdate));

	//messageSingleLine("Press up key to jump");
	messageDoubleLine("Hello!", "You are now Sticker Knight.");
	return true;
}

void Chapter0Level1::initMap(const std::string & tmxFile)
{
	GameScene::initMap(tmxFile);
}

void Chapter0Level1::initListener()
{
	GameScene::initListener();
}

void Chapter0Level1::heroUpdate(float dt)
{
	GameScene::heroUpdate(dt);
}

void Chapter0Level1::mapUpdate(float dt)
{
	GameScene::mapUpdate(dt);
}

void Chapter0Level1::regenerateUpdate(float dt)
{
	GameScene::regenerateUpdate(dt);
}

