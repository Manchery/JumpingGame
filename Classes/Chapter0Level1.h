#ifndef __CHAPTER0_LEVEL1_H__
#define __CHAPTER0_LEVEL1_H__

#include "cocos2d.h"
#include "Hero.h"
#include "Enemy.h"
#include "FollowEnemy.h"
#include "GameScene.h"
#include "common.h"

USING_NS_CC;

class Chapter0Level1 : public GameScene
{
public:
	static cocos2d::Scene* createScene();
	virtual void onEnterTransitionDidFinish() override;
	virtual bool init();
	virtual void initMap(const std::string &tmxFile) override;
	virtual void initListener() override;

	void heroUpdate(float dt) override;
	void mapUpdate(float dt) override;
	void regenerateUpdate(float dt) override;

	CREATE_FUNC(Chapter0Level1);
};

#endif // __CHAPTER0_LEVEL1_H__
