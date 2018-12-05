#ifndef __SLIDING_LAND_H__
#define __SLIDING_LAND_H__

#include "cocos2d.h"
USING_NS_CC;

class SlidingLand : public cocos2d::Sprite
{
public:
	static SlidingLand* create(const std::string& filename, float width, float height,float delay=0);

	bool initWithFile(const std::string& filename, float width, float height,float delay);
	void setTrack(float startx,float starty,float endx, float endy);
	void resetTrack();
	void slidingUpdate(float dt);
	void startSliding(float dt);
	float getStartX() { return startX; }
	float getStartY() { return startY; }
	float getEndX() { return endX; }
	float getEndY() { return endY; }
	bool isWait() {
		return delayTime < 0;
	}
	bool isLaunched() {
		return launched;
	}
	void setStopTime(float t) {
		stopTimeSet = t;
	}
	void setAttachedTrap(Sprite *trap) {
		trap->retain();
		attachedTrap = trap;
	}
protected:
	float startY,endY,startX, endX;
	float delayTime;
	float stopTime = 0,stopTimeSet;
	bool stoped = false;
	Vec2 tempVelocity;
	bool launched;
	Sprite *attachedTrap;
};

#endif // __SLIDING_LAND_H__
