#ifndef __HERO_H__
#define __HERO_H__

#include "cocos2d.h"

#define JUMP_LIMIT 2

enum HeroState {
	SILENCE = 0,
	RIGHT,
	LEFT,
	JUMP
};

class Hero : public cocos2d::Sprite
{
public:
	virtual bool init() override;
	CREATE_FUNC(Hero); 

	void update();
	
	void right();
	void left();
	void jump();
	void silence();

	HeroState getHeroState();
	int getJumpTimes();
	void setJumpTimes(int times);
	int getJumpLimit();
	void setJumpLimit(int times);
	
private:
	HeroState heroState;
	int jumpTimes,jumpLimit;
};

#endif // __HERO_H__
