#ifndef __TILE_TEST_SCENE_H__
#define __TILE_TEST_SCENE_H__

#include "cocos2d.h"
#include "GameScene.h"

USING_NS_CC;

class TileTestScene : public GameScene
{
public:
	static cocos2d::Scene* createScene();

	virtual void initMap();
	virtual void initListener();

	CREATE_FUNC(TileTestScene);
};

#endif // __TILE_TEST_SCENE_H__
