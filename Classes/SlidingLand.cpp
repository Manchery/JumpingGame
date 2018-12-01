#include "SlidingLand.h"
#include "common.h"
#include <cmath>
#include <algorithm>
USING_NS_CC;

SlidingLand* SlidingLand::create(const std::string& filename, float width, float height)
{
	SlidingLand *sprite = new (std::nothrow) SlidingLand();
	if (sprite && sprite->initWithFile(filename,width,height))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool SlidingLand::initWithFile(const std::string & filename, float width, float height)
{
	if (!Sprite::initWithFile(filename))
		return false;

	this->setContentSize(Size(width, height));

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

	this->setTag(SLIDING_LAND_T);

	return true;
}

void SlidingLand::setTrack(float startx, float starty, float endx, float endy)
{	
	startX = startx; startY = starty; endX = endx; endY = endy;
	this->setPosition(startX, startY);
	if (fabs(startX - endX) > 10.0f) {
		if (startX < endX)
			this->getPhysicsBody()->setVelocity(Vec2(100.0f, 100.0f*(endY - startY) / (fabs(endX - startX))));
		else
			this->getPhysicsBody()->setVelocity(Vec2(-100.0f, -100.0f*(endY - startY) / (fabs(endX - startX))));
	}
	else if (fabs(startY - endY) > 10.0f) {
		if (startY < endY)
			this->getPhysicsBody()->setVelocity(Vec2(100.0f*(endX - startX) / (fabs(endY - startY)),100.0f));
		else
			this->getPhysicsBody()->setVelocity(Vec2(-100.0f*(endX - startX) / (fabs(endY - startY)), -100.0f));
	}
	this->unscheduleAllCallbacks();
	this->schedule(schedule_selector(SlidingLand::slidingUpdate));
}

void SlidingLand::slidingUpdate(float dt)
{
	auto vel = this->getPhysicsBody()->getVelocity();
	if (stoped) {
		stopTime += dt;
		if (stopTime >= 1.0f) {
			this->getPhysicsBody()->setVelocity(Vec2(-tempVelocity.x, -tempVelocity.y));
			stopTime = 0; stoped = 0;
		}
	}
	else {
		if (fabs(startX - endX) > 10.0f) {
			float minX = std::min(startX, endX), maxX = std::max(startX, endX);
			if ((this->getPositionX() < minX && vel.x < 0) || (this->getPositionX() > maxX && vel.x > 0)) {
				tempVelocity = this->getPhysicsBody()->getVelocity();
				this->getPhysicsBody()->setVelocity(Vec2::ZERO);
				stoped = 1;
			}
		}else if (fabs(startY - endY) > 10.0f) {
			float minY = std::min(startY, endY), maxY = std::max(startY, endY);
			if ((this->getPositionY() < minY && vel.y < 0) || (this->getPositionY() > maxY && vel.y > 0)) {
				tempVelocity = this->getPhysicsBody()->getVelocity();
				this->getPhysicsBody()->setVelocity(Vec2::ZERO);
				stoped = 1;
			}
		}
	}
}


