#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "Hero.h"

USING_NS_CC;

class Enemy : public Hero
{
public:
	virtual bool init() override;
	CREATE_FUNC(Enemy);
	void setTrack(float y,float minX,float maxX);
	void resetPosition();
	void randomTravel(float dt);
	
protected:
	float posY, posMinX, posMaxX;
};

#endif // __HERO_H__
