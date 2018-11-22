#include "FollowEnemy.h"
#include "GameScene.h"
#include "common.h"

FollowEnemy* FollowEnemy::create(Hero *hero)
{
	FollowEnemy *sprite = new (std::nothrow) FollowEnemy();
	if (sprite && sprite->initWithHero(hero))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool FollowEnemy::initWithHero(Hero *hero) {
	if (!Sprite::init())
		return false;
	attachedHero = hero; attachedHero->retain();
	this->setAnchorPoint(attachedHero->getAnchorPoint());
	this->setPosition(attachedHero->getPosition());
	//this->setTexture(attachedHero->getTexture());
	this->setTexture("hero/HeroRightSilence.png");
	//this->setVisible(false);
	this->setOpacity(0);

	auto physicsSize = Size(this->getContentSize().width*0.75, this->getContentSize().height);
	auto physicsBody = PhysicsBody::createBox(physicsSize, PhysicsMaterial(0.1f, 0.0f, 0.0f));
	physicsBody->setGravityEnable(false);
	physicsBody->setRotationEnable(false);
	physicsBody->setCategoryBitmask(FOLLOW_ENEMY_M);
	physicsBody->setCollisionBitmask(HERO_M);
	physicsBody->setContactTestBitmask(0);
	this->setPhysicsBody(physicsBody);

	countDown = 5.0f; following = 0;

	this->setTag(FOLLOW_ENEMY_T);
	return true;
}

void FollowEnemy::startFollow(){
	this->schedule(schedule_selector(FollowEnemy::follow));
}
void FollowEnemy::stopFollow() {
	this->unscheduleAllCallbacks();

	this->getPhysicsBody()->setVelocity(Vec2::ZERO);

	this->setOpacity(0);

	if (following) {
		//this->setVisible(false);
		this->getPhysicsBody()->setContactTestBitmask(
			this->getPhysicsBody()->getContactTestBitmask() ^ HERO_M);
		attachedHero->getPhysicsBody()->setContactTestBitmask(
			attachedHero->getPhysicsBody()->getContactTestBitmask() ^ FOLLOW_ENEMY_M);

		following = 0;
	}

	while (!posQueue.empty()) posQueue.pop();
	//while (!velQueue.empty()) velQueue.pop();
	while (!texQueue.empty()) {
		texQueue.front()->release(); texQueue.pop();
	}
	countDown = 5.0f;
}

void FollowEnemy::follow(float dt)
{
	posQueue.push(attachedHero->getPosition());
	//velQueue.push(attachedHero->getPhysicsBody()->getVelocity());
	auto texture = attachedHero->getTexture();
	texture->retain();
	texQueue.push(texture);
	if (texQueue.size() > 100) {
		this->setPosition(posQueue.front()); 
		posQueue.pop();
		//this->getPhysicsBody()->setVelocity(velQueue.front());
		//velQueue.pop();
		this->setTexture(texQueue.front()); texQueue.front()->release(); texQueue.pop();
	}
	if (!following) {
		countDown -= dt;
		this->setOpacity((GLubyte)std::min(255,(int)((1.0f-countDown/5.0f)*255)));
		if (countDown <= 0.0f) {
			following = 1;
			this->getPhysicsBody()->setContactTestBitmask(
				this->getPhysicsBody()->getContactTestBitmask() ^ HERO_M);
			attachedHero->getPhysicsBody()->setContactTestBitmask(
				attachedHero->getPhysicsBody()->getContactTestBitmask() ^ FOLLOW_ENEMY_M);
			//this->setVisible(true);
			//this->setOpacity(255);
		}
	}
}

