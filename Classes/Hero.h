#ifndef __HERO_H__
#define __HERO_H__

#include "cocos2d.h"

#define JUMP_LIMIT 1

enum HeroState {
	RIGHT=0,
	RIGHTSILENCE,
	RIGHTJUMP,
	LEFT,
	LEFTSILENCE,
	LEFTJUMP,
	JUMP,
};

typedef int HeroType;
#define HERONORMAL 0,
#define HEROSHOT 1
#define HEROSHIELD 2

class GameScene;
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
	void rightJump();
	void leftJump();

	void shot();

	HeroState getHeroState();
	int getJumpTimes();
	void addJumpTimes();
	void resetJumpTimes();
	int getJumpLimit();
	void setJumpLimit(int times);
	void addInWater() {
		inWater++;
	}
	void decInWater() {
		inWater--;
	}
	void resetInWater() {
		inWater = 0;
	}
	int getInWater() {
		return inWater;
	}
	void addOnGround();
	void decOnGround();
	void resetOnGround();
	int getOnGround();
	HeroType getHeroType();
	void setHeroType(HeroType type);
	void setSlidingGround(cocos2d::Sprite *_slidingGround);
	void resetSlidingGround() {
		slidingGround = nullptr; 
	}
	cocos2d::Sprite *getSlidingGround();

	void shield();
	void unshield(float dt);
	void switchType();
	void switchTypeTo(int type);
	void switchTexture(int idx);
	bool getTypeUnlocked(int type);
	bool getShielded() {
		return shielded;
	}
	float getPhysicsMinX() {
		return this->getBoundingBox().getMidX() + minXOffset;
	}
	float getPhysicsMinY() {
		return this->getBoundingBox().getMidY() + minYOffset;
	}
	float getPhysicsMaxX() {
		return this->getBoundingBox().getMidX() + maxXOffset;
	}
	float getPhysicsMaxY() {
		return this->getBoundingBox().getMidY() + maxYOffset;
	}
	
protected:
	HeroState heroState,lastState;
	int jumpTimes,jumpLimit;
	int onGround,inWater;
	cocos2d::Animate *rightAnimate[4],*leftAnimate[4];
	cocos2d::Texture2D *rightTexture[4], *leftTexture[4], *jumpTexture[4], *rightJumpTexture[4],*leftJumpTexture[4];
	std::string bulletImage;
	HeroType heroType;
	cocos2d::Sprite *slidingGround;
	bool shielded;
	float lastShieldTime;
	float minXOffset, maxXOffset, minYOffset, maxYOffset;
};

#endif // __HERO_H__
