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
	void initResolutionSet();
	void initAudioSet();
	void initResetOption();
	void initResetGame();

	// implement the "static create()" method manually
	CREATE_FUNC(OptionScene);
private:
	CheckBox* resolutionCheckbox[4];
	Slider * volumnSlider;
	Button *resetOptionButton,*resetGameButton,*backButton;

};

#endif // __OPTION_SCENE_H__
