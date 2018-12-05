#include "SwingLand.h"
#include "GameScene.h"
#include "common.h"
#include <cmath>
USING_NS_CC;

const float PI = acos(-1.0);

SwingLand* SwingLand::create(const std::string& filename, float width, float height, bool trap)
{
	SwingLand *sprite = new (std::nothrow) SwingLand();
	if (sprite && sprite->initWithFile(filename, width, height, trap))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool SwingLand::initWithFile(const std::string & filename, float width, float height,bool trap)
{
	if (!Sprite::initWithFile(filename))
		return false;
	this->setContentSize(Size(width, height));

	auto physicsBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(1e9, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setCategoryBitmask(LAND_M);
	physicsBody->setCollisionBitmask(HERO_M | ENEMY_M | BULLET_M);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	this->setPhysicsBody(physicsBody);

	if (!trap) {
		clock = state = 0;
		this->schedule(schedule_selector(SwingLand::swingUpdate));
		this->setTag(SWING_LAND_T);
	}
	else{
		this->getPhysicsBody()->setAngularVelocity(PI);
		this->setTag(SWING_TRAP_T);
	}

	//this->setTag(SWING_LAND_T);
	stopTime = 4.0f;
	swingTime = 8.0f;

	return true;
}

void SwingLand::swingClockwise()
{
	this->getPhysicsBody()->setAngularVelocity(PI);
}

void SwingLand::swingAnticlockwise()
{
	this->getPhysicsBody()->setAngularVelocity(-PI);
}

void SwingLand::stopSwing()
{
	this->getPhysicsBody()->setAngularVelocity(0.0f);
	this->setRotation(0.0f);
}

void SwingLand::swingUpdate(float dt)
{
	clock += dt;
	if (((state==0 || state==2) && clock >= stopTime) ||
		((state == 1 || state == 3) && clock >= swingTime)) {
		clock = 0;
		state=(state+1)%4;
		if (state == 0 || state == 2) {
			stopSwing();
		}
		else if (state == 1) {
			swingClockwise();
		}
		else if (state == 3) {
			swingAnticlockwise();
		}
	}
}
