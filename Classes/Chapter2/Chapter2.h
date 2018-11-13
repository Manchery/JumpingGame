#ifndef __CHAPTER2_H__
#define __CHAPTER2_H__

#include "cocos2d.h"

class Chapter2 : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuExitCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Chapter2);
};

#endif // __CHAPTER2_H__
