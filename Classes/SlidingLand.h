#ifndef __SLIDING_LAND_H__
#define __SLIDING_LAND_H__

#include "cocos2d.h"

class SlidingLand : public cocos2d::Sprite
{
public:
	static SlidingLand* create(const std::string& filename);

	virtual bool initWithFile(const std::string& filename) override;
	void setTrack(float startx,float starty,float endx, float endy);
	void update(float dt);
	void setVelocity(cocos2d::Vec2 vel);
	
protected:
	float startY,endY,startX, endX;
};

#endif // __SLIDING_LAND_H__
