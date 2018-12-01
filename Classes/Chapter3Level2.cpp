#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Chapter3Level2.h"
#include "Chapter3Level3.h"
#include "HelloScene.h"
#include "SwingLand.h"
#include "SlidingLand.h"
using namespace CocosDenshion;

cocos2d::Scene * Chapter3Level2::createScene()
{
	return Chapter3Level2::create();
}

bool Chapter3Level2::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	initMap("map/chapter3Level2.tmx", Color4B::Color4B(39, 185, 154, 255));
	coinTotal = chapterCoinTotal[0];
	commonInitAfterMap();

	reverseSliding = false;
	//logUserDefault();
	return true;
}

void Chapter3Level2::switchScene(float dt)
{
	Chapter3Level3* scene = (Chapter3Level3*)Chapter3Level3::createScene();
	scene->setCoinCount(this->coinCount);
	scene->getHero()->switchTypeTo(hero->getHeroType());
	scene->setRunningTime(runningTime);
	Director::getInstance()->replaceScene(TransitionCrossFade::create(2.0f, scene));
}

void Chapter3Level2::gamePass()
{
	switchScene(0.0f); // or win()
}

void Chapter3Level2::heroDie() {
	GameScene::heroDie();
	if (hero->getPositionX() > mapSize.width / 2 && !reverseSliding) {
		reverseSliding = true;
		auto slidingLand1 = ((SlidingLand*)(frontGroundLayer->getChildByName("SlidingLand1"))),
			slidingLand2 = ((SlidingLand*)(frontGroundLayer->getChildByName("SlidingLand2")));
		slidingLand1->setTrack(slidingLand1->getEndX(),slidingLand1->getStartY(),
			slidingLand1->getStartX(), slidingLand1->getEndY()); 
		slidingLand2->setTrack(slidingLand2->getEndX(), slidingLand2->getStartY(),
			slidingLand2->getStartX(), slidingLand2->getEndY());
	}
	else {
		auto slidingLand1 = ((SlidingLand*)(frontGroundLayer->getChildByName("SlidingLand1"))),
			slidingLand2 = ((SlidingLand*)(frontGroundLayer->getChildByName("SlidingLand2")));
		slidingLand1->resetTrack();
		slidingLand2->resetTrack();
	}
}

