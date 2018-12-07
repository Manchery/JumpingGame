#ifndef __ENTRY_SCENE_H__
#define __ENTRY_SCENE_H__
#include "cocos2d.h"

USING_NS_CC;

class EntryScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	Node * chapterEntry(int idx);

	void enterChapter(int idx);

	bool onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event);

	bool onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event);

	void nextChapter();
	void previousChapter();
	CREATE_FUNC(EntryScene);
private:
	std::vector<Node*> chapters;
	Node *curChapter;
	int chapterTotal,iter;
	static int chapterPage;
};

#endif // __ENTRY_SCENE_H__
