#ifndef __PAUSE_SCENE_H__
#define __PAUSE_SCENE_H__

#include "cocos2d.h"
#include "TempScene.h"
#include "HelloScene.h"

USING_NS_CC;

class PauseScene : public TempScene
{
public:
	static cocos2d::Scene* createScene(int chapterId);
	virtual void onEnterTransitionDidFinish();
	virtual bool init(int chapterId);
	bool onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event);
	bool onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event);
	void setArrowPosition(int pos);
	void restart();
private:
	int chapterID;
	std::vector<MenuItemImage*> menuItems;
	Menu* menu;
	int curSelect;
};

#endif // __PAUSE_SCENE_H__
