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
	virtual void initMap(const std::string & tmxFile, const Color4B & backgroundColor) override;
	virtual void initListener() override;
	virtual void initBackgroundMusic() override;

	virtual void heroUpdate(float dt) override;
	virtual void mapUpdate(float dt) override;
	virtual void regenerateUpdate(float dt) override;
	virtual void messageUpdate(float dt) override;

	virtual void switchScene(float dt) override;
	virtual void gamePass() override;

	CREATE_FUNC(Chapter0Level1);
private:
	bool toldMove,toldCoin,toldExit;
};

#endif // __CHAPTER0_LEVEL1_H__
