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
	
protected:
	float startY,endY,startX, endX;
	float delayTime;
	float stopTime = 0;
	bool stoped = false;
	Vec2 tempVelocity;
};

#endif // __SLIDING_LAND_H__
