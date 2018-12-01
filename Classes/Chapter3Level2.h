#ifndef __CHAPTER3_LEVEL2_H__
#define __CHAPTER3_LEVEL2_H__

#include "cocos2d.h"
#include "Hero.h"
#include "Enemy.h"
#include "FollowEnemy.h"
#include "GameScene.h"
#include "common.h"

USING_NS_CC;

class Chapter3Level2 : public GameScene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void switchScene(float dt) override;
	virtual void gamePass() override;

	virtual void heroDie() override;

	CREATE_FUNC(Chapter3Level2);
private:
	bool reverseSliding;
};

#endif // __CHAPTER3_LEVEL2_H__
