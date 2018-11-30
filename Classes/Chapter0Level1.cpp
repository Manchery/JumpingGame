#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Chapter0Level1.h"
#include "Chapter0Level2.h"
#include "HelloScene.h"
#include "SwingLand.h"
using namespace CocosDenshion;

cocos2d::Scene * Chapter0Level1::createScene()
{
	return Chapter0Level1::create();
}

bool Chapter0Level1::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	initMap("map/chapter0Level1.tmx", Color4B::Color4B(39, 185, 154, 255));
	coinTotal = chapterCoinTotal[0];
	commonInitAfterMap();

	//logUserDefault();
	return true;
}

void Chapter0Level1::messageUpdate(float dt)
{
	if (!toldHello) {
		messageDoubleLine("Hello!", "You are now Sticker Knight.");
		toldHello = 1;
	}
}

void Chapter0Level1::switchScene(float dt)
{
	Chapter0Level2* scene=(Chapter0Level2*)Chapter0Level2::createScene();
	scene->setCoinCount(this->coinCount);
	scene->getHero()->switchTypeTo(hero->getHeroType());
	scene->setRunningTime(runningTime);
	Director::getInstance()->replaceScene(TransitionCrossFade::create(2.0f,scene));
}

void Chapter0Level1::gamePass()
{
	switchScene(0.0f); // or win()
}

