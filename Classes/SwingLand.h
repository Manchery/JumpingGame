#ifndef __SWING_LAND_H__
#define __SWING_LAND_H__

#include "cocos2d.h"
USING_NS_CC;

class SwingLand : public cocos2d::Sprite
{
public:
	static SwingLand* create(const std::string& filename);

	virtual bool initWithFile(const std::string& filename) override;
	void swing();
	void swingClockwise();
	void swingAnticlockwise();
	void stopSwing();
	void update(float dt);
	PhysicsJointPin *pin;
private:
};

#endif // __SWING_LAND_H__
