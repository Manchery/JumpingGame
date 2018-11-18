#include "Hero.h"
#include "common.h"
USING_NS_CC;

bool Hero::init() {
	if (!Sprite::init())
		return false;

	this->setTexture("hero/Blue_Front1.png");

	auto physicsBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
	physicsBody->setGravityEnable(true);
	physicsBody->setRotationEnable(false);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	//physicsBody->setCategoryBitmask(0x01);    // 0001
	//physicsBody->setCollisionBitmask(0x06);   // 0110
	this->setPhysicsBody(physicsBody);

	jumpTimes = 0;
	jumpLimit = JUMP_LIMIT;
	heroState = SILENCE;
	onGround = 0;

	auto rightAnimation = Animation::create();
	for (int i = 1; i <= 3; i++)
		rightAnimation->addSpriteFrameWithFile("hero/Blue_Right" + std::to_string(i) + ".png");
	rightAnimation->setDelayPerUnit(0.15f);
	rightAnimate = Animate::create(rightAnimation);
	rightAnimate->retain();


	auto leftAnimation = Animation::create();
	for (int i = 1; i <= 3; i++)
		leftAnimation->addSpriteFrameWithFile("hero/Blue_Left" + std::to_string(i) + ".png");
	leftAnimation->setDelayPerUnit(0.15f);
	leftAnimate = Animate::create(leftAnimation);
	leftAnimate->retain();

	auto frontAnimation = Animation::create();
	for (int i = 1; i <= 3; i++)
		frontAnimation->addSpriteFrameWithFile("hero/Blue_Front" + std::to_string(i) + ".png");
	frontAnimation->setDelayPerUnit(0.15f);
	frontAnimate = Animate::create(frontAnimation);
	frontAnimate->retain();

	rightTexture = Sprite::create("hero/Blue_Right1.png")->getTexture(); rightTexture->retain();
	leftTexture = Sprite::create("hero/Blue_Left1.png")->getTexture(); leftTexture->retain();
	frontTexture = Sprite::create("hero/Blue_Front1.png")->getTexture(); frontTexture->retain();
	backTexture = Sprite::create("hero/Blue_Back1.png")->getTexture(); backTexture->retain();
	bulletImage = "hero/star.png";

	return true;
}

void Hero::right() {
	if (heroState != RIGHT) {
		this->stopAllActions();
		this->runAction(RepeatForever::create(rightAnimate->clone()));
		heroState = RIGHT;
	}
}
void Hero::left() {
	if (heroState != LEFT) {
		this->stopAllActions();
		this->runAction(RepeatForever::create(leftAnimate->clone()));
		heroState = LEFT;
	}
}
void Hero::rightSilence(){
	this->stopAllActions();
	this->setTexture(rightTexture);
	heroState = RIGHTSILENCE;
}
void Hero::leftSilence() {
	this->stopAllActions();
	this->setTexture(leftTexture);
	heroState = LEFTSILENCE;
}
void Hero::jump() {
	if (heroState != JUMP) {
		this->stopAllActions();
		this->runAction(RepeatForever::create(frontAnimate->clone()));
		heroState = JUMP;
	}
}
void Hero::sleep(){
	this->stopAllActions();
	this->setTexture(backTexture);
}
void Hero::shot() {
	auto bullet = Sprite::create(bulletImage);
	bullet->setTag(BULLET_T);

	auto physicsBody = PhysicsBody::createBox(bullet->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
	physicsBody->setGravityEnable(false);
	physicsBody->setRotationEnable(false);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	bullet->setPhysicsBody(physicsBody);

	auto parent = this->getParent();
	auto heroPosition = Vec2(this->getPosition().x+this->getContentSize().width/2,
		this->getPosition().y+this->getContentSize().height/2);
	if (heroState == LEFT || heroState == LEFTSILENCE) {
		float sx = heroPosition.x - this->getContentSize().width / 2 - bullet->getContentSize().width / 2 - 10.0f;
		float sy = heroPosition.y;
		bullet->setPosition(sx, sy);
		bullet->getPhysicsBody()->setVelocity(Vec2(-500, 0));
	}
	else {
		float sx = heroPosition.x + this->getContentSize().width / 2 + bullet->getContentSize().width / 2 + 10.0f;
		float sy = heroPosition.y;
		bullet->setPosition(sx, sy);
		bullet->getPhysicsBody()->setVelocity(Vec2(500, 0));
	}
	parent->addChild(bullet, 10);
}
void Hero::silence() {
	this->stopAllActions();
	this->setTexture(frontTexture);
	heroState = SILENCE;
}

HeroState Hero::getHeroState() { return heroState; }
HeroType Hero::getHeroType() { return heroType; }
void Hero::setHeroType(HeroType type){ heroType = type; }

int Hero::getJumpTimes() { return jumpTimes; }
void Hero::addJumpTimes() { jumpTimes++; }
void Hero::resetJumpTimes() { jumpTimes = 0;  }
int Hero::getJumpLimit() { return jumpLimit; }
void Hero::setJumpLimit(int times) { jumpLimit = times; }

void Hero::setOnGround() { onGround = 1; }
void Hero::resetOnGround() { onGround = 0; }
bool Hero::getOnGround(){ return onGround; }

