#include "Hero.h"
#include "common.h"
#include "GameScene.h"
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
	physicsBody->setCollisionBitmask(ENEMY_M | LAND_M | BULLET_M | DOOR_M | COIN_M | EXIT_M | GAME_KEY_M | WATER_M);
	physicsBody->setContactTestBitmask(ENEMY_M | LAND_M | BULLET_M | DOOR_M | DOOR_KEY_M | COIN_M | EXIT_M | GAME_KEY_M | WATER_M);
	//physicsBody->setCollisionBitmask(0x06);   // 0110
	this->setPhysicsBody(physicsBody);

	jumpTimes = 0;
	jumpLimit = JUMP_LIMIT;
	heroState = RIGHTSILENCE;
	onGround = 0;

	std::string suf[] = { "","Shot","Shield","Protected"};
	for (int i = 0; i < 4; i++) {
		auto rightAnimation = Animation::create();
		for (int j = 1; j <= 4; j++)
			rightAnimation->addSpriteFrameWithFile("hero/HeroRight" + std::to_string(j) + suf[i]+".png");
		rightAnimation->setDelayPerUnit(0.2f);
		rightAnimate[i] = Animate::create(rightAnimation);
		rightAnimate[i]->retain();

		auto leftAnimation = Animation::create();
		for (int j = 1; j <= 4; j++)
			leftAnimation->addSpriteFrameWithFile("hero/HeroLeft" + std::to_string(j) + suf[i] + ".png");
		leftAnimation->setDelayPerUnit(0.2f);
		leftAnimate[i] = Animate::create(leftAnimation);
		leftAnimate[i]->retain();

		rightTexture[i] = Sprite::create("hero/HeroRightSilence"+ suf[i] + ".png")->getTexture(); rightTexture[i]->retain();
		leftTexture[i] = Sprite::create("hero/HeroLeftSilence" + suf[i] + ".png")->getTexture(); leftTexture[i]->retain();
		jumpTexture[i] = Sprite::create("hero/HeroJump" + suf[i] + ".png")->getTexture(); jumpTexture[i]->retain();
		rightJumpTexture[i] = Sprite::create("hero/HeroRightJump" + suf[i] + ".png")->getTexture(); rightJumpTexture[i]->retain();
		leftJumpTexture[i] = Sprite::create("hero/HeroLeftJump" + suf[i] + ".png")->getTexture(); leftJumpTexture[i]->retain();
	}

	bulletImage = "hero/bullet.png";
	lastShieldTime = -100.0f;
	this->setTag(HERO_T);

	return true;
}

void Hero::right() {
	if (heroState != RIGHT) {
		this->stopAllActions();
		this->runAction(RepeatForever::create(rightAnimate[heroType+shielded]->clone()));
		if (heroState != RIGHT) lastState = heroState;
		heroState = RIGHT;
	}
}
void Hero::left() {
	if (heroState != LEFT) {
		this->stopAllActions();
		this->runAction(RepeatForever::create(leftAnimate[heroType + shielded]->clone()));
		if (heroState != LEFT) lastState = heroState;
		heroState = LEFT;
	}
}
void Hero::rightSilence(){
	this->stopAllActions();
	this->setTexture(rightTexture[heroType + shielded]);
	if (heroState != RIGHTSILENCE) lastState = heroState;
	heroState = RIGHTSILENCE;
}
void Hero::leftSilence() {
	this->stopAllActions();
	this->setTexture(leftTexture[heroType + shielded]);
	if (heroState != LEFTSILENCE) lastState = heroState;
	heroState = LEFTSILENCE;
}
void Hero::jump() {
	this->stopAllActions();
	this->setTexture(jumpTexture[heroType + shielded]);
	if (heroState != JUMP) lastState = heroState;
	heroState = JUMP;
}
void Hero::rightJump() {
	this->stopAllActions();
	this->setTexture(rightJumpTexture[heroType + shielded]);
	if (heroState != RIGHTJUMP) lastState = heroState;
	heroState = RIGHTJUMP;
}
void Hero::leftJump() {
	this->stopAllActions();
	this->setTexture(leftJumpTexture[heroType + shielded]);
	if (heroState != LEFTJUMP) lastState = heroState;
	heroState = LEFTJUMP;
}
void Hero::switchType(){
	shielded = false;
	this->unscheduleAllCallbacks();
	(++heroType) %= 3;
	while (!getTypeUnlocked(heroType)) (++heroType) %= 3;
	switchTexture(heroType);
}
void Hero::switchTypeTo(int type) {
	shielded = false;
	this->unscheduleAllCallbacks();
	heroType = type;
	switchTexture(heroType);
}
void Hero::switchTexture(int idx){
	this->stopAllActions();
	if (heroState == RIGHT) { this->runAction(RepeatForever::create(rightAnimate[idx]->clone())); }
	else if (heroState == LEFT) { this->runAction(RepeatForever::create(leftAnimate[idx]->clone())); }
	else if (heroState == RIGHTSILENCE) { this->setTexture(rightTexture[idx]); }
	else if (heroState == LEFTSILENCE) { this->setTexture(leftTexture[idx]); }
	else if (heroState == JUMP) { this->setTexture(jumpTexture[idx]); }
	else if (heroState == RIGHTJUMP) { this->setTexture(rightJumpTexture[idx]); }
	else if (heroState == LEFTJUMP) { this->setTexture(leftJumpTexture[idx]); }
}
bool Hero::getTypeUnlocked(int type){
	if (!type) return true;
	GameScene *scene = (GameScene*)(Director::getInstance()->getRunningScene());
	if (type == 1) {
		return UserDefault::getInstance()->getBoolForKey("canShot") || scene->getGotShot();
	}
	else if (type == 2) {
		return UserDefault::getInstance()->getBoolForKey("canShield") || scene->getGotShield();
	}
	return false;
}
void Hero::shot() {
	if (heroType != HEROSHOT) return;
	auto bullet = Sprite::create(bulletImage);
	bullet->setTag(BULLET_T);

	auto physicsBody = PhysicsBody::createBox(bullet->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
	physicsBody->setGravityEnable(false);
	physicsBody->setRotationEnable(false);
	physicsBody->setCategoryBitmask(BULLET_M);
	physicsBody->setCollisionBitmask(0xFFFFFFFF ^ COIN_M);
	physicsBody->setContactTestBitmask(0xFFFFFFFF^COIN_M);
	bullet->setPhysicsBody(physicsBody);

	auto parent = this->getParent();
	auto heroPosition = Vec2(this->getPosition().x+this->getContentSize().width/2,
		this->getPosition().y+this->getContentSize().height/2);
	if ((heroState==JUMP && (lastState == LEFT || lastState == LEFTSILENCE || lastState == LEFTJUMP))
		|| heroState == LEFT || heroState == LEFTSILENCE || heroState == LEFTJUMP) {
		float sx = heroPosition.x - this->getContentSize().width / 2 - bullet->getContentSize().width / 2 - 10.0f;
		float sy = heroPosition.y;
		bullet->setFlippedX(true);
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

void Hero::shield() {
	if (heroType != HEROSHIELD) return;
	if (shielded) return;
	float runningTime = ((GameScene*)(Director::getInstance()->getRunningScene()))->getRunningTime();
	if (runningTime - lastShieldTime < 5.0f) return;
	lastShieldTime = runningTime;
	shielded = true;
	switchTexture(3);
	this->scheduleOnce(schedule_selector(Hero::unshield),3.0f);
}
void Hero::unshield(float dt){
	switchTexture(HEROSHIELD);
	shielded = false;
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

