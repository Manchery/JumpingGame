#ifndef __ENTRY_SCENE_H__
#define __ENTRY_SCENE_H__
#include "cocos2d.h"

class EntryScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	Node * chapterEntry(int idx);

	CREATE_FUNC(EntryScene);
private:
};

#endif // __ENTRY_SCENE_H__
