#ifndef __BOSS_H__
#define __BOSS_H__

#include "cocos2d.h"
#include "Hero.h"

USING_NS_CC;

class Boss : public Sprite
{
public:
	virtual bool init(float width,float height);
	static Boss* create(float width, float height);
	bool damaged();
	void comeBack(float ft);
private:
	bool damagable;
	Vec2 position;
	int hp;
};

#endif // __BOSS_H__
