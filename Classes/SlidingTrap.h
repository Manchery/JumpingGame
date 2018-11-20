#ifndef __SLIDING_TRAP_H__
#define __SLIDING_TRAP_H__

#include "cocos2d.h"
USING_NS_CC;

class SlidingTrap : public cocos2d::Sprite
{
public:
	static SlidingTrap* create(const std::string& filename);

	virtual bool initWithFile(const std::string& filename) override;
	void setTrack(float posx, float starty, float endy);
	void update(float dt);
	
protected:
	float startY,endY,posX;

	float stopTime = 0;
	bool stoped = false;
	Vec2 tempVelocity;
};

#endif // __SLIDING_TRAP_H__
