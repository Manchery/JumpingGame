#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Chapter2Level1.h"
#include "HelloScene.h"
#include "SwingLand.h"
#include "Door.h"
using namespace CocosDenshion;

cocos2d::Scene * Chapter2Level1::createScene()
{
	return Chapter2Level1::create();
}

bool Chapter2Level1::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	initMap("map/chapter2Level1.tmx",Color4B::Color4B(53,73,94,255));
	coinTotal = chapterCoinTotal[2];
	commonInitAfterMap();

	return true;
}

void Chapter2Level1::switchScene(float dt)
{
	Director::getInstance()->replaceScene(HelloScene::createScene());
}

void Chapter2Level1::gamePass()
{
	win();
	auto userData = UserDefault::getInstance();
	userData->setBoolForKey("chapter2Pass",true);
	auto record= userData->getIntegerForKey("chapter2CoinCount");
	userData->setIntegerForKey("chapter2CoinCount", std::max(record,coinCount));
}

