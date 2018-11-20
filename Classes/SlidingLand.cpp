#include "SlidingLand.h"
#include "common.h"
#include <cmath>
#include <algorithm>
USING_NS_CC;

SlidingLand* SlidingLand::create(const std::string& filename)
{
	SlidingLand *sprite = new (std::nothrow) SlidingLand();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool SlidingLand::initWithFile(const std::string & filename)
{
	if (!Sprite::initWithFile(filename))
		return false;

	auto physicsBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
	//physicsBody->setGravityEnable(false);
	//physicsBody->setRotationEnable(false);
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(LAND_M);
	physicsBody->setCollisionBitmask(HERO_M | ENEMY_M | BULLET_M);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	//physicsBody->setCategoryBitmask(0x01);    // 0001
	//physicsBody->setCollisionBitmask(0x06);   // 0110
	this->setPhysicsBody(physicsBody);

	return true;
}

void SlidingLand::setTrack(float startx, float starty, float endx, float endy)
{	
	startX = startx; startY = starty; endX = endx; endY = endy;
	this->setPosition(startX, startY);
	if (startX < endX)
		this->getPhysicsBody()->setVelocity(Vec2(50.0f,50.0f*(endY-startY)/(fabs(endX-startX))));
	else
		this->getPhysicsBody()->setVelocity(Vec2(-50.0f, -50.0f*(endY - startY) / (fabs(endX - startX))));
	this->unscheduleAllCallbacks();
	this->schedule(schedule_selector(SlidingLand::update));
}

void SlidingLand::update(float dt)
{
	float minX = std::min(startX,endX), maxX = std::max(startX,endX);
	auto vel = this->getPhysicsBody()->getVelocity();
	if (stoped) {
		stopTime += dt;
		if (stopTime >= 1.0f) {
			this->getPhysicsBody()->setVelocity(Vec2(-tempVelocity.x, -tempVelocity.y));
			stopTime = 0; stoped = 0;
		}
	}
	else {
		if ((this->getPositionX() < minX && vel.x < 0) || (this->getPositionX() > maxX && vel.x > 0)) {
			tempVelocity = this->getPhysicsBody()->getVelocity();
			this->getPhysicsBody()->setVelocity(Vec2::ZERO);
			stoped = 1;
		}
	}
}


