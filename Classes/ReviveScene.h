#ifndef __REVIVE_SCENE_H__
#define __REVIVE_SCENE_H__

#include "cocos2d.h"
#include "TempScene.h"

USING_NS_CC;

class ReviveScene : public TempScene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void goBack(float dt);
	CREATE_FUNC(ReviveScene);
};

#endif // __REVIVE_SCENE_H__
