#ifndef __DOOR_H__
#define __DOOR_H__

#include "cocos2d.h"
USING_NS_CC;

class Door : public cocos2d::Sprite
{
public:
	static Door* create(const std::string& filename);

	virtual bool initWithFile(const std::string& filename) override;
	void setTrack(float posx, float starty, float endy);

	void move();
	
protected:
	float startY,endY,posX;
	int state;
};

class DoorKey : public cocos2d::Sprite
{
public:
	static DoorKey* create(const std::string& filename);

	virtual bool initWithFile(const std::string& filename) override;
	void setDoor(Door *_door);
	void lock();

protected:
	Door* door;
};

#endif // __SLIDING_TRAP_H__
