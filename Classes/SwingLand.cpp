#include "SwingLand.h"
#include "GameScene.h"
#include "common.h"
#include <cmath>
USING_NS_CC;

const float PI = acos(-1.0);

SwingLand* SwingLand::create(const std::string& filename)
{
	SwingLand *sprite = new (std::nothrow) SwingLand();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool SwingLand::initWithFile(const std::string & filename)
{
	if (!Sprite::initWithFile(filename))
		return false;

	auto physicsBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(1e6, 0.0f, 0.0f));
	physicsBody->setDynamic(true);
	physicsBody->setAngularDamping(0.0f);
	physicsBody->setCategoryBitmask(LAND_M);
	physicsBody->setCollisionBitmask(HERO_M | ENEMY_M | BULLET_M);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	this->setPhysicsBody(physicsBody);

	this->setTag(SWING_LAND_T);

	return true;
}

void SwingLand::swing(GameScene* scene)
{
	//auto scene = (GameScene*)(Director::getInstance()->getRunningScene());
	scene->getPhysicsWorld()->removeJoint(pin);
	this->unscheduleAllCallbacks();

	auto boundBody=scene->getBoundBody();
	auto pos = this->getPosition() + this->getParent()->getPosition();
	pin = PhysicsJointPin::construct(this->getPhysicsBody(), boundBody, pos);
	scene->getPhysicsWorld()->addJoint(pin);

	clock = state = 0;
	this->schedule(schedule_selector(SwingLand::update));
}

void SwingLand::swingClockwise()
{
	this->getPhysicsBody()->setAngularVelocity(2*PI);
}

void SwingLand::swingAnticlockwise()
{
	this->getPhysicsBody()->setAngularVelocity(-2 * PI);
}

void SwingLand::stopSwing()
{
	this->getPhysicsBody()->setAngularVelocity(0.0f);
	this->setRotation(0.0f);
}

void SwingLand::update(float dt)
{
	clock += dt;
	if (((state==0 || state==2) && clock >= 4.0f) ||
		((state == 1 || state == 3) && clock >= 8.0f)) {
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
