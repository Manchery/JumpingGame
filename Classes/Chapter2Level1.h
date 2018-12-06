#ifndef __CHAPTER2_LEVEL1_H__
#define __CHAPTER2_LEVEL1_H__

#include "cocos2d.h"
#include "Hero.h"
#include "Enemy.h"
#include "FollowEnemy.h"
#include "GameScene.h"
#include "common.h"

USING_NS_CC;

class Chapter2Level1 : public GameScene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void messageUpdate(float dt) override;

	virtual void initBackgroundMusic() override;

	virtual void drawMap(const TMXTiledMap * tileMap) override;

	virtual bool onContactBegin(cocos2d::PhysicsContact &contact) override;

	virtual void gamePass() override;

	CREATE_FUNC(Chapter2Level1);
private:
	bool toldHello;
};

#endif // __CHAPTER2_LEVEL1_H__
