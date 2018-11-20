#include "Enemy.h"

bool Enemy::init() {
	if (!Sprite::init())
		return false;

	this->setTexture("hero/Dark_Front1.png");

	auto physicsBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
	physicsBody->setGravityEnable(true);
	physicsBody->setRotationEnable(false);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	//physicsBody->setCategoryBitmask(0x01);    // 0001
	//physicsBody->setCollisionBitmask(0x06);   // 0110
	this->setPhysicsBody(physicsBody);

	heroState = SILENCE;

	auto rightAnimation = Animation::create();
	for (int i = 1; i <= 3; i++)
		rightAnimation->addSpriteFrameWithFile("hero/Dark_Right" + std::to_string(i) + ".png");
	rightAnimation->setDelayPerUnit(0.15f);
	rightAnimate = Animate::create(rightAnimation);
	rightAnimate->retain();

	auto leftAnimation = Animation::create();
	for (int i = 1; i <= 3; i++)
		leftAnimation->addSpriteFrameWithFile("hero/Dark_Left" + std::to_string(i) + ".png");
	leftAnimation->setDelayPerUnit(0.15f);
	leftAnimate = Animate::create(leftAnimation);
	leftAnimate->retain();

	auto frontAnimation = Animation::create();
	for (int i = 1; i <= 3; i++)
		frontAnimation->addSpriteFrameWithFile("hero/Dark_Front" + std::to_string(i) + ".png");
	frontAnimation->setDelayPerUnit(0.15f);
	frontAnimate = Animate::create(frontAnimation);
	frontAnimate->retain();

	rightTexture = Sprite::create("hero/Dark_Right1.png")->getTexture(); rightTexture->retain();
	leftTexture = Sprite::create("hero/Dark_Left1.png")->getTexture(); leftTexture->retain();
	frontTexture = Sprite::create("hero/Dark_Front1.png")->getTexture(); frontTexture->retain();
	backTexture = Sprite::create("hero/Dark_Back1.png")->getTexture(); backTexture->retain();

	this->schedule(schedule_selector(Enemy::randomTravel), 4.0f);
	
	return true;
}

#define RANDNUM(l,r) (CCRANDOM_0_1()*(r-l)+l)

void Enemy::setTrack(float y, float minX, float maxX){
	posY = y; posMinX = minX; posMaxX = maxX;
	this->setPosition(RANDNUM(posMinX, posMaxX),posY);
}
void Enemy::resetPosition(){
	this->setPosition(RANDNUM(posMinX, posMaxX), posY);
}
void Enemy::randomTravel(float dt) {
	int dice = rand() % 5;
	float velocity = 200.0f;
	Vec2 position = this->getPosition();
	if (dice == 0) {
		silence();
	}
	else if (dice % 2 == 0) { //right
		float finalX = RANDNUM(position.x, posMaxX);
		//float finalX = posMaxX;
		float duration = (finalX - position.x) / velocity;
		auto moveTo = MoveTo::create(duration, Vec2(finalX, position.y));
		//auto repeat = Repeat::create(rightAnimate, duration / 0.45f);
		right();
		this->runAction(moveTo);
	}
	else {
		float finalX = RANDNUM(posMinX,position.x);
		//float finalX = posMinX;
		float duration = (position.x-finalX) / velocity;
		auto moveTo = MoveTo::create(duration, Vec2(finalX, position.y));
		//auto repeat = Repeat::create(rightAnimate, duration / 0.45f);
		left();
		this->runAction(moveTo);
	}
}
