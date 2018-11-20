#ifndef __HERO_H__
#define __HERO_H__

#include "cocos2d.h"

#define JUMP_LIMIT 2

enum HeroState {
	SILENCE = 0,
	RIGHT,
	RIGHTSILENCE,
	LEFTSILENCE,
	LEFT,
	JUMP,
};

enum HeroType {
	NORMAL=0,
	SHOT
};

class Hero : public cocos2d::Sprite
{
public:
	virtual bool init() override;
	CREATE_FUNC(Hero); 
	
	void right();
	void left();
	void rightSilence();
	void leftSilence();
	void jump();
	void silence();
	void sleep();

	void shot();

	HeroState getHeroState();
	int getJumpTimes();
	void addJumpTimes();
	void resetJumpTimes();
	int getJumpLimit();
	void setJumpLimit(int times);
	void setOnGround();
	void resetOnGround();
	int getOnGround();
	HeroType getHeroType();
	void setHeroType(HeroType type);
	void setSlidingGround(cocos2d::Sprite *_slidingGround);
	cocos2d::Sprite *getSlidingGround();
	
protected:
	HeroState heroState;
	int jumpTimes,jumpLimit;
	int onGround;
	cocos2d::Animate *rightAnimate,*leftAnimate,*frontAnimate;
	cocos2d::Texture2D *rightTexture, *leftTexture, *frontTexture, *backTexture;
	std::string bulletImage;
	HeroType heroType;
	cocos2d::Sprite *slidingGround;
};

#endif // __HERO_H__
