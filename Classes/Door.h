#ifndef __DOOR_H__
#define __DOOR_H__

#include "cocos2d.h"
USING_NS_CC;

class Door : public cocos2d::Sprite
{
public:
	static Door* create(const std::string& filename, float width, float height);

	bool initWithFile(const std::string& filename, float width, float height);
	void setTrack(float posx, float starty, float endy);

	void move();
	void setAttachedTrap(Sprite *trap) {
		trap->retain();
		attachedTrap = trap;
	}
	
protected:
	float startY,endY,posX;
	int state;
	Sprite *attachedTrap;
};

class DoorKey : public cocos2d::Sprite
{
public:
	static DoorKey* create(float width, float height);

	virtual bool initWithFile(float width, float height);
	void addDoor(Door *_door);
	void lock();

	void enableAgain(float dt);

protected:
	std::vector<Door*> doors;
	Size contentSize;
	bool enabled;
	int state;
};

#endif // __SLIDING_TRAP_H__
