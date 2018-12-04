#include "Boss.h"
#include "common.h"
#include "GameScene.h"

Boss* Boss::create(float width, float height)
{
	Boss *sprite = new (std::nothrow) Boss();
	if (sprite && sprite->init(width, height))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool Boss::init(float width, float height) {
	if (!Sprite::init())
		return false;

	this->setTexture("map/boss.png");
	this->setContentSize(Size(width, height));

	auto physicsSize = Size(this->getContentSize().width*0.8, this->getContentSize().height*0.8);
	auto physicsBody = PhysicsBody::createBox(physicsSize, PhysicsMaterial(0.1f, 0.0f, 0.0f));
	physicsBody->setGravityEnable(false);
	physicsBody->setRotationEnable(false);
	physicsBody->setCategoryBitmask(BOSS_M);
	physicsBody->setCollisionBitmask(BULLET_M | HERO_M);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	this->setPhysicsBody(physicsBody);
	
	this->setTag(BOSS_T);
	this->hp = 5;
	this->damagable = 1;
	
	return true;
}

bool Boss::damaged()
{
	if (!damagable) return false;

	damagable = false;
	
	if (!(--hp))
		return true; // win

	Size visibleSize = Director::getInstance()->getVisibleSize();
	position = this->getPosition();
	auto moveTo = MoveTo::create(2.0f,Vec2(visibleSize.width, position.y));
	this->runAction(moveTo);
	this->scheduleOnce(schedule_selector(Boss::comeBack), 2.0f+3.0f);
	return false;
}

void Boss::comeBack(float ft)
{
	this->runAction(MoveTo::create(2.0f, position));
	damagable = true;
}
