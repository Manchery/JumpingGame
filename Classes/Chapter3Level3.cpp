#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Chapter3Level3.h"
#include "HelloScene.h"
#include "SwingLand.h"
#include "SlidingLand.h"
using namespace CocosDenshion;

cocos2d::Scene * Chapter3Level3::createScene()
{
	return Chapter3Level3::create();
}

bool Chapter3Level3::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	chapterID = 3;

	initMap("map/chapter3Level3.tmx", Color4B::Color4B(39, 185, 154, 255));
	coinTotal = chapterCoinTotal[3];
	commonInitAfterMap();

	for (int i = 1; i <= 6; i++) {
		auto slidingLand = (SlidingLand*)(backGroundLayer->getChildByName("SlidingLand" + std::to_string(i)));
		auto trap = (Sprite*)(frontGroundLayer->getChildByName("Trap" + std::to_string(i)));
		trap->retain();
		slidingLand->setAttachedTrap(trap);
	}

	//logUserDefault();
	return true;
}

void Chapter3Level3::gamePass()
{
	win();
}