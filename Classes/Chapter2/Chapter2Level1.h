#ifndef __CHAPTER2_LEVEL1_H__
#define __CHAPTER2_LEVEL1_H__

#include "cocos2d.h"

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

	void heroRight();
	void heroLeft();
	void heroJump();
	void heroSilence();
	// implement the "static create()" method manually
	CREATE_FUNC(Chapter2Level1);

private:
	int heroJumpTimes;
	cocos2d::Sprite *hero;
	//cocos2d::Animation *rightAnimation, *leftAnimation, *frontAnimation;
	int heroState;
	bool upKeyDown, leftKeyDown, rightKeyDown, downKeyDown;
};

#endif // __CHAPTER2_LEVEL1_H__
