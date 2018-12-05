#ifndef __HELLO_SCENE_H__
#define __HELLO_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelloScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	bool onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event);

	bool onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event);

	// implement the "static create()" method manually
	CREATE_FUNC(HelloScene);
};

#endif // __HELLO_SCENE_H__
