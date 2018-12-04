#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Chapter6Level1.h"
#include "HelloScene.h"
#include "SwingLand.h"
#include "SlidingLand.h"
#include "Boss.h"
using namespace CocosDenshion;

cocos2d::Scene * Chapter6Level1::createScene()
{
	return Chapter6Level1::create();
}

bool Chapter6Level1::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	initMap("map/chapter6Level1.tmx", Color4B::Color4B(135, 242, 243, 255));
	coinTotal = chapterCoinTotal[6];
	commonInitAfterMap();

	return true;
}

void Chapter6Level1::switchScene(float dt)
{
	Director::getInstance()->replaceScene(HelloScene::createScene());
}

void Chapter6Level1::gamePass()
{
	win();
	auto userData = UserDefault::getInstance();
	userData->setBoolForKey("chapter5Pass", true);
	auto record = userData->getIntegerForKey("chapter5CoinCount");
	userData->setIntegerForKey("chapter5CoinCount", std::max(record, coinCount));
}

void Chapter6Level1::mapUpdate(float dt){
	GameScene::mapUpdate(dt);
	if (coinCount == coinTotal)
		gamePass();
}