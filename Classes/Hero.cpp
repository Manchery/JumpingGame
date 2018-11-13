#include "Hero.h"
USING_NS_CC;

bool Hero::init() {
	if (!Sprite::init())
		return false;

	this->setTexture("hero/Blue_Front1.png");

	auto physicsBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
	physicsBody->setGravityEnable(true);
	physicsBody->setRotationEnable(false);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	physicsBody->setCategoryBitmask(0x01);    // 0001
	physicsBody->setCollisionBitmask(0x06);   // 0110
	this->setPhysicsBody(physicsBody);

	jumpTimes = 0;
	jumpLimit = JUMP_LIMIT;
	heroState = SILENCE;

	return true;
}

void Hero::right() {
	if (heroState != RIGHT) {
		auto animation = Animation::create();
		for (int i = 1; i <= 3; i++)
			animation->addSpriteFrameWithFile("hero/Blue_Right" + std::to_string(i) + ".png");
		animation->setDelayPerUnit(0.15f);
		this->stopAllActions();
		this->runAction(RepeatForever::create(Animate::create(animation)));
		heroState = RIGHT;
	}
}
void Hero::left() {
	if (heroState != LEFT) {
		auto animation = Animation::create();
		for (int i = 1; i <= 3; i++)
			animation->addSpriteFrameWithFile("hero/Blue_Left" + std::to_string(i) + ".png");
		animation->setDelayPerUnit(0.15f);
		this->stopAllActions();
		this->runAction(RepeatForever::create(Animate::create(animation)));
		heroState = LEFT;
	}
}
void Hero::jump() {
	if (heroState != JUMP) {
		auto animation = Animation::create();
		for (int i = 1; i <= 3; i++)
			animation->addSpriteFrameWithFile("hero/Blue_Front" + std::to_string(i) + ".png");
		animation->setDelayPerUnit(0.15f);
		this->stopAllActions();
		this->runAction(RepeatForever::create(Animate::create(animation)));
		heroState = JUMP;
	}
}
void Hero::silence() {
	this->stopAllActions();
	this->setTexture("hero/Blue_Front1.png");
	heroState = SILENCE;
}

HeroState Hero::getHeroState() { return heroState; }
int Hero::getJumpTimes() { return jumpTimes; }
void Hero::setJumpTimes(int times) { jumpTimes=times; }
int Hero::getJumpLimit() { return jumpLimit; }
void Hero::setJumpLimit(int times) { jumpLimit = times; }