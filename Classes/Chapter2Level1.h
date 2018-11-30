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

	virtual void switchScene(float dt) override;
	virtual void gamePass() override;

	CREATE_FUNC(Chapter2Level1);
};

#endif // __CHAPTER2_LEVEL1_H__