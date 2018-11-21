#ifndef __FOLLOW_ENEMY_H__
#define __FOLLOW_ENEMY_H__

#include "cocos2d.h"
#include "Hero.h"

USING_NS_CC;

class FollowEnemy : public Sprite
{
public:

	static FollowEnemy* create(Hero *hero);

	virtual bool initWithHero(Hero *hero);
	void startFollow();
	void stopFollow();
	void follow(float dt);
	
protected:
	Hero *attachedHero;
	std::queue<Vec2> posQueue;
	//std::queue<Vec2> velQueue;
	std::queue<Texture2D*> texQueue;
	float countDown;
	bool following;
};

#endif // __HERO_H__
