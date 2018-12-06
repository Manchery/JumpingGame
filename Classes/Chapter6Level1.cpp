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

	chapterID = 6;

	initMap("map/chapter6Level1.tmx", Color4B::Color4B(135, 242, 243, 255));
	coinTotal = chapterCoinTotal[6];
	commonInitAfterMap();

	return true;
}


void Chapter6Level1::messageUpdate(float dt)
{
	if (!toldHello && runningTime >= 2.0f) {
		messageDoubleLine("A fancy land full of diamonds and happiness,", "collect them !");
		toldHello = 1;
	}
}

void Chapter6Level1::gamePass()
{
	win();
}

void Chapter6Level1::mapUpdate(float dt){
	GameScene::mapUpdate(dt);
	if (coinCount == coinTotal)
		gamePass();
}