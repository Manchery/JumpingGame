#ifndef __OPTION_SCENE_H__
#define __OPTION_SCENE_H__
#include "ui/CocosGUI.h"
#include "cocos2d.h"
using namespace cocos2d::ui;

class OptionScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void addResolutionSet();
	void addAudioSet();
	void addResetOptions();
	void addResetGame();

	// implement the "static create()" method manually
	CREATE_FUNC(OptionScene);
private:
	CheckBox* resolutionCheckbox[4];
	Slider * volumnSlider;

};

#endif // __OPTION_SCENE_H__
