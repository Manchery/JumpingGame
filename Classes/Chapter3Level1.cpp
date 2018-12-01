#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Chapter3Level2.h"
#include "Chapter3Level1.h"
#include "HelloScene.h"
#include "SwingLand.h"
using namespace CocosDenshion;

cocos2d::Scene * Chapter3Level1::createScene()
{
	return Chapter3Level1::create();
}

bool Chapter3Level1::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	initMap("map/chapter3Level1.tmx", Color4B::Color4B(53, 73, 94, 255));
	coinTotal = chapterCoinTotal[0];
	commonInitAfterMap();

	//logUserDefault();
	return true;
}

void Chapter3Level1::switchScene(float dt)
{
	Chapter3Level2* scene = (Chapter3Level2*)Chapter3Level2::createScene();
	scene->setCoinCount(this->coinCount);
	scene->getHero()->switchTypeTo(hero->getHeroType());
	scene->setRunningTime(runningTime);
	Director::getInstance()->replaceScene(TransitionCrossFade::create(2.0f, scene));
}

void Chapter3Level1::gamePass()
{
	switchScene(0.0f); // or win()
}

