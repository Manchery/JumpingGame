#include "SlidingLand.h"
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
	this->unscheduleAllSelectors();
	this->schedule(schedule_selector(SlidingLand::update));
}

void SlidingLand::update(float dt)
{
	static float stopTime = 0;
	static Vec2 tempVelocity;
	float minX = startX, maxX = endX;
	if (minX > maxX) std::swap(minX, maxX);
	if ((this->getPositionX()<minX) ||
		(this->getPositionX() + this->getContentSize().width>maxX)) {
		if (stopTime == 0) {
			tempVelocity = this->getPhysicsBody()->getVelocity();
			this->getPhysicsBody()->setVelocity(Vec2::ZERO);
		}
		stopTime += dt;
		//log("%f",stopTime);
		if (stopTime >= 1.0f) {
			this->getPhysicsBody()->setVelocity(Vec2(-tempVelocity.x, -tempVelocity.y));
			stopTime = 0;
		}
	}
}

void SlidingLand::setVelocity(cocos2d::Vec2 vel) {
	this->getPhysicsBody()->setVelocity(vel);
}


