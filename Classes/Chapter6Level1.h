#ifndef __CHAPTER6_LEVEL1_H__
#define __CHAPTER6_LEVEL1_H__

#include "cocos2d.h"
#include "Hero.h"
#include "Enemy.h"
#include "FollowEnemy.h"
#include "GameScene.h"
#include "common.h"

USING_NS_CC;

class Chapter6Level1 : public GameScene // Chapter Bonus
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void switchScene(float dt) override;
	virtual void gamePass() override;
	virtual void mapUpdate(float ft) override;

	CREATE_FUNC(Chapter6Level1);
};

#endif // __CHAPTER6_LEVEL1_H__
