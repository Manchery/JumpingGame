#ifndef __CHAPTER3_LEVEL3_H__
#define __CHAPTER3_LEVEL3_H__

#include "cocos2d.h"
#include "Hero.h"
#include "Enemy.h"
#include "FollowEnemy.h"
#include "GameScene.h"
#include "common.h"

USING_NS_CC;

class Chapter3Level3 : public GameScene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void gamePass() override;

	CREATE_FUNC(Chapter3Level3);
};

#endif // __CHAPTER3_LEVEL3_H__
