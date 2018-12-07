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
bool Chapter0Level2::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	chapterID = 0;

	initMap("map/chapter0Level2.tmx",Color4B::Color4B(53,73,94,255));
	coinTotal = chapterCoinTotal[0];
	commonInitAfterMap();

	return true;
}

void Chapter0Level2::initBackgroundMusic()
{
	BGM(CHAPTER0BGM);
}

void Chapter0Level2::gamePass()
{
	win();
}

