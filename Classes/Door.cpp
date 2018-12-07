#include "Door.h"
#include "common.h"
#include <cmath>
#include <algorithm>
USING_NS_CC;

Door* Door::create(const std::string& filename, float width, float height)
{
	Door *sprite = new (std::nothrow) Door();
	if (sprite && sprite->initWithFile(filename,width,height))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool Door::initWithFile(const std::string & filename, float width, float height)
{
	if (!Sprite::initWithFile(filename))
		return false;
	this->setContentSize(Size(width, height));

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
		this->stopAllActions();
		auto moveBy = MoveBy::create(
			2.0f*fabs(this->getPositionY() - endY) / fabs(endY - startY), Vec2(0, endY - this->getPositionY()));
		this->runAction(moveBy);
		if (attachedTrap!=nullptr){
			attachedTrap->stopAllActions();
			attachedTrap->runAction(moveBy->clone());
		}
	}
	else {
		this->stopAllActions();
		auto moveBy = MoveBy::create(
			2.0f*fabs(this->getPositionY() - startY) / fabs(endY - startY), Vec2(0, startY - this->getPositionY()));
		this->runAction(moveBy);
		if (attachedTrap != nullptr) {
			attachedTrap->stopAllActions();
			attachedTrap->runAction(moveBy->clone());
		}
	}
	state ^= 1;
}

DoorKey* DoorKey::create(float width, float height)
{
	DoorKey *sprite = new (std::nothrow) DoorKey();
	if (sprite && sprite->initWithFile(width,height))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool DoorKey::initWithFile(float width, float height)
{
	if (!Sprite::init())
		return false;
	
	this->setTexture("map/blobBlue.png");
	this->setContentSize(Size(width, height));

	auto physicsBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
	//physicsBody->setGravityEnable(false);
	//physicsBody->setRotationEnable(false);
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(DOOR_KEY_M);
	physicsBody->setCollisionBitmask(HERO_M | BULLET_M);
	physicsBody->setContactTestBitmask(HERO_M | BULLET_M);
	this->setPhysicsBody(physicsBody);

	this->setTag(DOOR_KEY_T);

	doors.clear();

	contentSize = this->getContentSize();
	enabled = true;

	return true;
}

void DoorKey::addDoor(Door * door)
{
	door->retain(); doors.push_back(door);
}

void DoorKey::lock(){
	if (!enabled) return;
	for (auto door:doors)
		door->move();
	if (state ^= 1)
		this->setTexture("map/blobGreen.png");
	else
		this->setTexture("map/blobBlue.png");
	this->setContentSize(contentSize);
	enabled = false;
	this->scheduleOnce(schedule_selector(DoorKey::enableAgain), 1.0f);
}

void DoorKey::enableAgain(float dt)
{
	//this->setTexture("map/blobBlue.png");
	this->setContentSize(contentSize);
	enabled = true;
}
