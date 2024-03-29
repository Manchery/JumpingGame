#ifndef __OPTION_SCENE_H__
#define __OPTION_SCENE_H__
#include "ui/CocosGUI.h"
#include "cocos2d.h"
using namespace cocos2d::ui;

USING_NS_CC;

class OptionScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene(bool canResetGame=0);

	virtual bool init(bool canResetGame);
	void initResolutionSet();
	//void initAudioSet();
	void initResetOption();
	void initResetGame();

	bool onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event);

	bool onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event);

private:
	CheckBox* resolutionCheckbox[4];
	//Slider * volumnSlider;
	Button *resetOptionButton,*resetGameButton,*backButton;

};

#endif // __OPTION_SCENE_H__
