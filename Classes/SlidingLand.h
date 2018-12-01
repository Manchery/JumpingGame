#ifndef __SLIDING_LAND_H__
#define __SLIDING_LAND_H__

#include "cocos2d.h"
USING_NS_CC;

class SlidingLand : public cocos2d::Sprite
{
public:
	static SlidingLand* create(const std::string& filename, float width, float height);

	bool initWithFile(const std::string& filename, float width, float height);
	void setTrack(float startx,float starty,float endx, float endy);
	void slidingUpdate(float dt);
	
protected:
	float startY,endY,startX, endX;

	float stopTime = 0;
	bool stoped = false;
	Vec2 tempVelocity;
};

#endif // __SLIDING_LAND_H__
