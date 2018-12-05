#ifndef __SWING_LAND_H__
#define __SWING_LAND_H__

#include "cocos2d.h"
USING_NS_CC;

class GameScene;

class SwingLand : public cocos2d::Sprite
{
public:
	static SwingLand* create(const std::string& filename,float width,float height, bool trap=false);

	virtual bool initWithFile(const std::string& filename, float width, float height, bool trap);
	void swingClockwise();
	void swingAnticlockwise();
	void stopSwing();
	void swingUpdate(float dt);
	void setSwingTime(float t) {
		swingTime = t;
	}
	void setStopTime(float t) {
		stopTime = t;
	}
private:
	float clock;
	int state;
	float swingTime, stopTime;
};

#endif // __SWING_LAND_H__
