#ifndef __TEMP_SCENE_H__
#define __TEMP_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class TempScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(TempScene);
protected:
	cocos2d::Sprite *background;
};

#endif // __TEMP_SCENE_H__
