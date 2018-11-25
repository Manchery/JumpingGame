#include "Hero.h"
#include "common.h"
USING_NS_CC;

bool Hero::init() {
	if (!Sprite::init())
		return false;

	this->setTexture("hero/HeroRightSilence.png");

	auto physicsSize = Size(this->getContentSize().width*0.75, this->getContentSize().height*0.8);
	auto physicsBody = PhysicsBody::createBox(physicsSize, PhysicsMaterial(0.1f, 0.0f, 0.0f));
	physicsBody->setPositionOffset(Vec2(0, -this->getContentSize().height*0.2/2));
	//physicsBody->setLinearDamping(0.2f);
	physicsBody->setGravityEnable(true);
	physicsBody->setRotationEnable(false);
	physicsBody->setCategoryBitmask(HERO_M);
	physicsBody->setCollisionBitmask(ENEMY_M | LAND_M | BULLET_M | DOOR_M | COIN_M | EXIT_M | GAME_KEY_M);
	physicsBody->setContactTestBitmask(ENEMY_M | LAND_M | BULLET_M | DOOR_M | DOOR_KEY_M | COIN_M | EXIT_M | GAME_KEY_M);
	//physicsBody->setCollisionBitmask(0x06);   // 0110
	this->setPhysicsBody(physicsBody);

	jumpTimes = 0;
	jumpLimit = JUMP_LIMIT;
	heroState = SILENCE;
	onGround = 0;

	auto rightAnimation = Animation::create();
	for (int i = 1; i <= 4; i++)
		rightAnimation->addSpriteFrameWithFile("hero/HeroRight" + std::to_string(i) + ".png");
	rightAnimation->setDelayPerUnit(0.2f);
	rightAnimate = Animate::create(rightAnimation);
	rightAnimate->retain();


	auto leftAnimation = Animation::create();
	for (int i = 1; i <= 4; i++)
		leftAnimation->addSpriteFrameWithFile("hero/HeroLeft" + std::to_string(i) + ".png");
	leftAnimation->setDelayPerUnit(0.2f);
	leftAnimate = Animate::create(leftAnimation);
	leftAnimate->retain();

	rightTexture = Sprite::create("hero/HeroRightSilence.png")->getTexture(); rightTexture->retain();
	leftTexture = Sprite::create("hero/HeroLeftSilence.png")->getTexture(); leftTexture->retain();
	jumpTexture = Sprite::create("hero/HeroJump.png")->getTexture(); jumpTexture->retain();
	rightJumpTexture = Sprite::create("hero/HeroRightJump.png")->getTexture(); rightJumpTexture->retain();
	leftJumpTexture = Sprite::create("hero/HeroLeftJump.png")->getTexture(); leftJumpTexture->retain();
	//backTexture = Sprite::create("hero/Blue_Back1.png")->getTexture(); backTexture->retain();
	bulletImage = "hero/bullet.png";

	this->setTag(HERO_T);

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
	this->stopAllActions();
	this->setTexture(jumpTexture);
	heroState = JUMP;
}
void Hero::rightJump() {
	this->stopAllActions();
	this->setTexture(rightJumpTexture);
	heroState = RIGHTJUMP;
}
void Hero::leftJump() {
	this->stopAllActions();
	this->setTexture(leftJumpTexture);
	heroState = LEFTJUMP;
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
	if (heroState == LEFT || heroState == LEFTSILENCE || heroState == LEFTJUMP) {
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

HeroState Hero::getHeroState() { return heroState; }
HeroType Hero::getHeroType() { return heroType; }
void Hero::setHeroType(HeroType type){ heroType = type; }

void Hero::setSlidingGround(cocos2d::Sprite *_slidingGround){ slidingGround = _slidingGround; }
cocos2d::Sprite * Hero::getSlidingGround(){ return slidingGround;}

int Hero::getJumpTimes() { return jumpTimes; }
void Hero::addJumpTimes() { jumpTimes++; }
void Hero::resetJumpTimes() { jumpTimes = 0;  }
int Hero::getJumpLimit() { return jumpLimit; }
void Hero::setJumpLimit(int times) { jumpLimit = times; }

void Hero::addOnGround() { onGround++; }
void Hero::decOnGround() { onGround--; }
void Hero::resetOnGround(){ onGround = 0; }
int Hero::getOnGround(){ return onGround; }

