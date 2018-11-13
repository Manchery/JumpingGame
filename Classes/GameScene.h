#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Hero.h"

class GameScene : public cocos2d::Scene
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	void initMap();
	//virtual bool initWithPhysics();

	// a selector callback
	void menuExitCallback(cocos2d::Ref* pSender);
	bool onContactBegin(const cocos2d::PhysicsContact &contact);
	bool onContactEnd(const cocos2d::PhysicsContact &contact);
	void heroUpdate(float dt);

	CREATE_FUNC(GameScene);

private:
	Hero *hero;
	bool upKeyDown, leftKeyDown, rightKeyDown, downKeyDown;
};

#endif // __GAME_SCENE_H__
