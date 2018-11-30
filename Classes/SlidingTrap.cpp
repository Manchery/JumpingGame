#include "SlidingTrap.h"
#include "common.h"
#include <cmath>
#include <algorithm>
USING_NS_CC;

SlidingTrap* SlidingTrap::create(const std::string& filename)
{
	SlidingTrap *sprite = new (std::nothrow) SlidingTrap();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool SlidingTrap::initWithFile(const std::string & filename)
{
	if (!Sprite::initWithFile(filename))
		return false;

	auto physicsBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
	//physicsBody->setGravityEnable(false);
	//physicsBody->setRotationEnable(false);
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(LAND_M);
	physicsBody->setCollisionBitmask(HERO_M | ENEMY_M | BULLET_M);
	//physicsBody->setContactTestBitmask(0xFFFFFFFF);
	physicsBody->setContactTestBitmask(HERO_M | ENEMY_M | BULLET_M);
	this->setPhysicsBody(physicsBody);

	this->setTag(SLIDING_TRAP_T);

	return true;
}

void SlidingTrap::setTrack(float posx, float starty, float endy)
{	
	posX = posx; startY = starty; endY = endy;

	this->setPosition(posX, startY);
	this->getPhysicsBody()->setVelocity(Vec2(0.0f,50.0f));
	//this->unscheduleAllCallbacks();
	this->schedule(schedule_selector(SlidingTrap::slidingUpdate));
}

void SlidingTrap::slidingUpdate(float dt)
{
	auto vel = this->getPhysicsBody()->getVelocity();
	if (stoped) {
		stopTime += dt;
		if (stopTime >= 5.0f) {
			this->getPhysicsBody()->setVelocity(Vec2(-tempVelocity.x, -tempVelocity.y));
			stopTime = 0; stoped = 0;
		}
	}
	else {
		if ((this->getPositionY() < startY && vel.y < 0) || (this->getPositionY() > endY && vel.y > 0)) {
			tempVelocity = this->getPhysicsBody()->getVelocity();
			this->getPhysicsBody()->setVelocity(Vec2::ZERO);
			stoped = 1;
		}
	}
}


