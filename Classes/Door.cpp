#include "Door.h"
#include "common.h"
#include <cmath>
#include <algorithm>
USING_NS_CC;

Door* Door::create(const std::string& filename)
{
	Door *sprite = new (std::nothrow) Door();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool Door::initWithFile(const std::string & filename)
{
	if (!Sprite::initWithFile(filename))
		return false;

	auto physicsBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
	//physicsBody->setGravityEnable(false);
	//physicsBody->setRotationEnable(false);
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(DOOR_M);
	physicsBody->setCollisionBitmask(HERO_M | ENEMY_M | BULLET_M);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	this->setPhysicsBody(physicsBody);
	
	this->setTag(DOOR_T);

	return true;
}

void Door::setTrack(float posx, float starty, float endy)
{
	posX = posx; startY = starty; endY = endy; state = 0;
	this->setPosition(posX, startY);
}

void Door::move() {
	if (state == 0) {
		auto moveBy = MoveBy::create(2, Vec2(0, endY - startY));
		this->runAction(moveBy);
	}
	else {
		auto moveBy = MoveBy::create(2, Vec2(0, -(endY - startY)));
		this->runAction(moveBy);
	}
	state ^= 1;
}

DoorKey* DoorKey::create(const std::string& filename)
{
	DoorKey *sprite = new (std::nothrow) DoorKey();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool DoorKey::initWithFile(const std::string & filename)
{
	if (!Sprite::initWithFile(filename))
		return false;

	auto physicsBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
	//physicsBody->setGravityEnable(false);
	//physicsBody->setRotationEnable(false);
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(DOOR_KEY_M);
	physicsBody->setCollisionBitmask(HERO_M | BULLET_M);
	physicsBody->setContactTestBitmask(HERO_M | BULLET_M);
	this->setPhysicsBody(physicsBody);

	this->setTag(DOOR_KEY_T);

	return true;
}

void DoorKey::setDoor(Door * _door){
	_door->retain(); door = _door;
}

void DoorKey::lock(){
	door->move();
}
