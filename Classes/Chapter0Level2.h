#ifndef __CHAPTER0_LEVEL2_H__
#define __CHAPTER0_LEVEL2_H__

#include "cocos2d.h"
#include "Hero.h"
#include "Enemy.h"
#include "FollowEnemy.h"
#include "GameScene.h"
#include "common.h"

USING_NS_CC;

class Chapter0Level2 : public GameScene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	virtual void gamePass() override;

	CREATE_FUNC(Chapter0Level2);
};

#endif // __CHAPTER0_LEVEL2_H__
