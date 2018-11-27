#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Chapter0Level2.h"
#include "HelloScene.h"
#include "SwingLand.h"
using namespace CocosDenshion;

cocos2d::Scene * Chapter0Level2::createScene()
{
	return Chapter0Level2::create();
}

void Chapter0Level2::onEnterTransitionDidFinish()
{
	GameScene::onEnterTransitionDidFinish();
}

bool Chapter0Level2::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	initMap("map/chapter0Level2.tmx",Color4B::Color4B(53,73,94,255));
	initBackgroundMusic();
	initDashboard();
	initListener();

	revivePoint = hero->getPosition();
	coinTotal = chapterCoinTotal[0]; coinCount = 0;
	heroDied = heroJumped = 0;
	gotGameKey = 0; needGameKey = 1;

	this->schedule(schedule_selector(GameScene::heroUpdate));
	this->schedule(schedule_selector(GameScene::mapUpdate));
	this->schedule(schedule_selector(GameScene::regenerateUpdate));

	return true;
}

void Chapter0Level2::initMap(const std::string & tmxFile, const Color4B & backgroundColor)
{
	GameScene::initMap(tmxFile, backgroundColor);
}

void Chapter0Level2::initListener()
{
	GameScene::initListener();
}

void Chapter0Level2::initBackgroundMusic()
{
}

void Chapter0Level2::heroUpdate(float dt)
{
	GameScene::heroUpdate(dt);
}

void Chapter0Level2::mapUpdate(float dt)
{
	GameScene::mapUpdate(dt);
}

void Chapter0Level2::regenerateUpdate(float dt)
{
	GameScene::regenerateUpdate(dt);
}

void Chapter0Level2::switchScene(float dt)
{
	Director::getInstance()->replaceScene(HelloScene::createScene());
}

void Chapter0Level2::gamePass()
{
	win();
	auto userData = UserDefault::getInstance();
	userData->setBoolForKey("chapter0Pass",true);
	auto record= userData->getIntegerForKey("chapter0CoinCount");
	userData->setIntegerForKey("chapter0CoinCount", std::max(record,coinCount));
}

