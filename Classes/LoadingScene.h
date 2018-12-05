#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"


class LoadingScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init() override;
	virtual void onEnterTransitionDidFinish() override;
	void loadingCallBack(cocos2d::Texture2D *texture);
	CREATE_FUNC(LoadingScene);
private:
	cocos2d::ProgressTimer *loadProgress;
	int totalNum; 
	int loadedNum; 
};

#endif // __LOADING_SCENE_H__
