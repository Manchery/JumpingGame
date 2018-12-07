#ifndef __HELP_SCENE_H__
#define __HELP_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelpScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	bool onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event);

	bool onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event);

	void nextPage();
	void previousPage();

	// implement the "static create()" method manually
	CREATE_FUNC(HelpScene);
private:
	int helpPageTotal,iter;
	std::vector<Texture2D*> helpPages;
};

#endif // __HELP_SCENE_H__
