#ifndef __CHAPTER3_H__
#define __CHAPTER3_H__

#include "cocos2d.h"

class Chapter3 : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuExitCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Chapter3);
};

#endif // __CHAPTER3_H__
