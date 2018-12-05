#ifndef __CHAPTER4_LEVEL1_H__
#define __CHAPTER4_LEVEL1_H__

#include "cocos2d.h"
#include "Hero.h"
#include "Enemy.h"
#include "FollowEnemy.h"
#include "GameScene.h"
#include "common.h"

USING_NS_CC;

class Chapter4Level1 : public GameScene
{
public:
	static cocos2d::Scene* createScene();
	virtual void onEnterTransitionDidFinish() override;
	virtual bool init();

	virtual void gamePass() override;

	CREATE_FUNC(Chapter4Level1);
};

#endif // __CHAPTER4_LEVEL1_H__
