#ifndef __CHAPTER4_H__
#define __CHAPTER4_H__

#include "cocos2d.h"

class Chapter4 : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuExitCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Chapter4);
};

#endif // __CHAPTER4_H__
