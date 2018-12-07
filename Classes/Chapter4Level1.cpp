#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Chapter4Level1.h"
#include "HelloScene.h"
#include "SwingLand.h"
using namespace CocosDenshion;

cocos2d::Scene * Chapter4Level1::createScene()
{
	return Chapter4Level1::create();
}

void Chapter4Level1::onEnterTransitionDidFinish()
{
	//flush
	hero->getPhysicsBody()->setVelocity(Vec2::ZERO);
	upKeyDown = leftKeyDown = rightKeyDown = downKeyDown = 0;
	//lastKey = EventKeyboard::KeyCode::KEY_NONE;
	heroJumped = heroBounced = 0;

	if (heroDied) {
		for (auto node : destroyedList) {
			auto sprite = (Sprite*)node;

			if (sprite->getTag() == COIN_T) continue; // make it easy

			frontGroundLayer->addChild(sprite, 1);
			if (sprite->getTag() == ENEMY_T) {
				Enemy *enemy = (Enemy*)node;
				enemy->resetPosition();
			}
			if (sprite->getTag() == GAME_KEY_T)
				gotGameKey = 0;
		}
		destroyedList.clear();

		if (followEnemy != nullptr) {
			followEnemy->stopFollow();
			followEnemy->startFollow(0.0f);
		}

		heroDied = 0;
	}
}

bool Chapter4Level1::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	chapterID = 4;

	initMap("map/chapter4Level1.tmx", Color4B::Color4B(53, 73, 94, 255));
	coinTotal = chapterCoinTotal[4];

	commonInitAfterMap();

	lastKey = EventKeyboard::KeyCode::KEY_LEFT_ARROW;

	followEnemy = FollowEnemy::create(hero);
	followEnemy->retain();
	frontGroundLayer->addChild(followEnemy, 1);

	//followEnemy->startFollow(0.0f);
	followEnemy->scheduleOnce(schedule_selector(FollowEnemy::startFollow),2.5f);

	hero->leftSilence();

	return true;
}

void Chapter4Level1::initBackgroundMusic()
{
	BGM(CHAPTER4BGM);
}

void Chapter4Level1::messageUpdate(float dt)
{
	if (!toldHello && runningTime>=2.0f) {
		messageDoubleLine("Do not abandon your shadow,", "Or he will chase you and kill you.");
		toldHello = 1;
	}
}

void Chapter4Level1::gamePass()
{
	win();
}

