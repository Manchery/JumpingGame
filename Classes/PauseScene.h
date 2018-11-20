#ifndef __PAUSE_SCENE_H__
#define __PAUSE_SCENE_H__

#include "cocos2d.h"
#include "TempScene.h"

USING_NS_CC;

class PauseScene : public TempScene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(PauseScene);
};

#endif // __PAUSE_SCENE_H__
