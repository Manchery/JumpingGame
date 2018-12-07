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

	chapterID = 3;

	initMap("map/chapter3Level1.tmx", Color4B::Color4B(53, 73, 94, 255));
	coinTotal = chapterCoinTotal[3];
	commonInitAfterMap();

	//logUserDefault();
	return true;
}

void Chapter3Level1::initBackgroundMusic()
{
	BGM(CHAPTER3BGM);
}

void Chapter3Level1::gamePass()
{
	nextLevel((Chapter3Level2*)Chapter3Level2::createScene());
}

