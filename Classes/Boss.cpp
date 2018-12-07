#include "Boss.h"
#include "common.h"
#include "GameScene.h"

#define BOSS_HP 5

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
	this->hp = BOSS_HP;
	this->damagable = 1;

	contentSize = this->getContentSize();
	
	return true;
}

void Boss::drop() {
	auto delay = DelayTime::create(0.5f);
	auto blink = Blink::create(0.1f, 1);
	auto seq = Sequence::create(delay, blink, delay->clone(), blink->clone(), delay->clone(), blink->clone(),
		delay->clone(), CallFunc::create(CC_CALLBACK_0(Boss::removeFromParent, this)), nullptr);
	this->runAction(seq);
}

bool Boss::damaged()
{
	if (!damagable) return false;

	this->setTexture("map/bossDamaged.png");
	this->setContentSize(contentSize);
	damagable = false;
	
	if (!(--hp)) {
		drop();
		return true; // win
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	position = this->getPosition();
	auto moveTo = MoveTo::create(2.0f,Vec2(visibleSize.width, position.y));
	this->runAction(moveTo);
	this->scheduleOnce(schedule_selector(Boss::comeBack), 2.0f+6.0f);

	EFFECT("ghostdeath.wav");

	return false;
}

void Boss::setDamagable(float dt) {
	damagable = true;
	this->setTexture("map/boss.png");
	this->setContentSize(contentSize);

	//EFFECT("ghostbirth.wav");
}

void Boss::comeBack(float ft)
{
	this->runAction(MoveTo::create(2.0f, position));
	this->scheduleOnce(schedule_selector(Boss::setDamagable), 2.5f);
}

void Boss::resetHP()
{
	hp = BOSS_HP;
}
