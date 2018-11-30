#include "Enemy.h"
#include "common.h"
#include "GameScene.h"

bool Enemy::init() {
	if (!Sprite::init())
		return false;

	this->setTexture("hero/EnemyRightSilence.png");

	auto physicsSize = Size(this->getContentSize().width*0.75, this->getContentSize().height*0.8);
	auto physicsBody = PhysicsBody::createBox(physicsSize, PhysicsMaterial(0.1f, 0.0f, 0.0f));
	physicsBody->setPositionOffset(Vec2(0, -this->getContentSize().height*0.2 / 2));
	physicsBody->setGravityEnable(true);
	physicsBody->setRotationEnable(false);
	physicsBody->setCategoryBitmask(ENEMY_M);
	physicsBody->setCollisionBitmask(HERO_M | LAND_M | BULLET_M | DOOR_M);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	this->setPhysicsBody(physicsBody);

	heroState = RIGHTSILENCE;
	heroType = 0;

	auto rightAnimation = Animation::create();
	for (int i = 1; i <= 4; i++)
		rightAnimation->addSpriteFrameWithFile("hero/EnemyRight" + std::to_string(i) + ".png");
	rightAnimation->setDelayPerUnit(0.2f);
	rightAnimate[0] = Animate::create(rightAnimation);
	rightAnimate[0]->retain();


	auto leftAnimation = Animation::create();
	for (int i = 1; i <= 4; i++)
		leftAnimation->addSpriteFrameWithFile("hero/EnemyLeft" + std::to_string(i) + ".png");
	leftAnimation->setDelayPerUnit(0.2f);
	leftAnimate[0] = Animate::create(leftAnimation);
	leftAnimate[0]->retain();

	rightTexture[0] = Sprite::create("hero/EnemyRightSilence.png")->getTexture(); rightTexture[0]->retain();
	leftTexture[0] = Sprite::create("hero/EnemyLeftSilence.png")->getTexture(); leftTexture[0]->retain();

	this->schedule(schedule_selector(Enemy::randomTravel), 4.0f);

	this->setTag(ENEMY_T);
	
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
		return;
	}
	else if (dice % 2 == 0) { //right
		float finalX = RANDNUM(position.x, posMaxX);
		//float finalX = posMaxX;
		float duration = (finalX - position.x) / velocity;
		auto moveTo = MoveTo::create(duration, Vec2(finalX, position.y));
		//auto repeat = Repeat::create(rightAnimate, duration / 0.45f);
		right();
		this->runAction(Sequence::create(moveTo, 
			CallFunc::create(CC_CALLBACK_0(Enemy::rightSilence, this)),nullptr));
	}
	else {
		float finalX = RANDNUM(posMinX,position.x);
		//float finalX = posMinX;
		float duration = (position.x-finalX) / velocity;
		auto moveTo = MoveTo::create(duration, Vec2(finalX, position.y));
		//auto repeat = Repeat::create(rightAnimate, duration / 0.45f);
		left();
		this->runAction(Sequence::create(moveTo,
			CallFunc::create(CC_CALLBACK_0(Enemy::leftSilence, this)), nullptr));
	}
}