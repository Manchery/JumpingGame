#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Chapter0Level1.h"
#include "Chapter0Level2.h"
#include "HelloScene.h"
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

	initMap("map/chapter0Level1.tmx", Color4B::Color4B(39, 185, 154, 255));
	initBackgroundMusic();
	initDashboard();
	initListener();

	revivePoint = hero->getPosition();
	coinTotal = chapterCoinTotal[0]; coinCount = 0;
	heroDied = heroJumped = 0;
	gotGameKey = 0; needGameKey = 0;

	this->schedule(schedule_selector(GameScene::heroUpdate));
	this->schedule(schedule_selector(GameScene::mapUpdate));
	this->schedule(schedule_selector(GameScene::regenerateUpdate));

	//messageSingleLine("Press up key to jump");
	messageDoubleLine("Hello!", "You are now Sticker Knight.");
	return true;
}

void Chapter0Level1::initMap(const std::string & tmxFile, const Color4B & backgroundColor)
{
	GameScene::initMap(tmxFile, backgroundColor);
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

void Chapter0Level1::switchScene(float dt)
{
	Chapter0Level2* scene=(Chapter0Level2*)Chapter0Level2::createScene();
	scene->setCoinCount(this->coinCount);
	Director::getInstance()->replaceScene(scene);
}

void Chapter0Level1::gamePass()
{
	switchScene(0.0f); // or win()
}

