#ifndef __CHAPTER5_LEVEL1_H__
#define __CHAPTER5_LEVEL1_H__

#include "cocos2d.h"
#include "Hero.h"
#include "Enemy.h"
#include "FollowEnemy.h"
#include "GameScene.h"
#include "common.h"

USING_NS_CC;

class Chapter5Level1 : public GameScene // Chapter Final
{
public:
	static cocos2d::Scene* createScene();
	virtual void onEnterTransitionDidFinish() override;

	virtual bool init();
	virtual void messageUpdate(float dt) override;
	virtual void initBackgroundMusic() override;
	void drawBoss(const std::string & tmxFile);

	virtual void gamePass() override;

	virtual void mapUpdate(float dt) override;
	virtual bool onContactBegin(cocos2d::PhysicsContact &contact) override;

	CREATE_FUNC(Chapter5Level1);
private:
	int trapCnt;
	bool firstLoad;
	bool toldHello;
};

#endif // __CHAPTER5_LEVEL1_H__
