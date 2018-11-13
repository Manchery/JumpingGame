#ifndef __CHAPTER2_LEVEL1_H__
#define __CHAPTER2_LEVEL1_H__

#include "cocos2d.h"
#include "Hero.h"

class Chapter2Level1 : public cocos2d::Scene
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	//virtual bool initWithPhysics();

	// a selector callback
	void menuExitCallback(cocos2d::Ref* pSender);
	bool onContactBegin(const cocos2d::PhysicsContact &contact);
	bool onContactEnd(const cocos2d::PhysicsContact &contact);
	void heroUpdate(float dt);

	CREATE_FUNC(Chapter2Level1);

private:
	Hero *hero;
	bool upKeyDown, leftKeyDown, rightKeyDown, downKeyDown;
};

#endif // __CHAPTER2_LEVEL1_H__
