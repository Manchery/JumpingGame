#ifndef __TILE_TEST_SCENE_H__
#define __TILE_TEST_SCENE_H__

#include "cocos2d.h"
#include "GameScene.h"

USING_NS_CC;

class TileTestScene : public Scene
{
public:
	static cocos2d::Scene* createScene();

	void drawBackGround(ValueVector & arrObj, int zOrder);

	void drawMap(ValueVector & arrObj, int zOrder);

	virtual bool init() override;

	CREATE_FUNC(TileTestScene);
private:
	Layer *frontGroundLayer, *backGroundLayer;
};

#endif // __TILE_TEST_SCENE_H__
