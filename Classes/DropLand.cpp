#include "DropLand.h"
#include "GameScene.h"
USING_NS_CC;

DropLand* DropLand::create(const std::string& filename)
{
	DropLand *sprite = new (std::nothrow) DropLand();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool DropLand::initWithFile(const std::string & filename)
{
	if (!Sprite::initWithFile(filename))
		return false;

	auto physicsBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(LAND_M);
	physicsBody->setCollisionBitmask(HERO_M | ENEMY_M | BULLET_M);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);
	//physicsBody->setCategoryBitmask(0x01);    // 0001
	//physicsBody->setCollisionBitmask(0x06);   // 0110
	this->setPhysicsBody(physicsBody);

	this->setTag(DROP_LAND_T);

	return true;
}
void DropLand::remove() {
	this->retain();
	auto scene = (GameScene*)Director::getInstance()->getRunningScene();
	scene->addRenerate(this);
	//this->removeFromParent();
	//this->schedule(schedule_selector(DropLand::regenerate));
}
void DropLand::drop() {
	/*auto tintTo0 = TintTo::create(0.0f, 255.0f, 255.0f, 255.0f);
	auto tintTo1 = TintTo::create(0.1f, 120.0f, 232.0f, 254.0f);
	auto tintTo2 = TintTo::create(0.1f, 232.0f, 120.0f, 254.0f);
	auto tintTo3 = TintTo::create(0.1f, 232.0f, 254.0f, 120.0f);
	*/
	auto delay = DelayTime::create(0.5f);
	auto blink=Blink::create(0.1f, 1);
	auto seq = Sequence::create(delay, blink, delay->clone(), blink, delay->clone(), blink,
		delay->clone(), blink,CallFunc::create(CC_CALLBACK_0(DropLand::remove,this)),nullptr);
	//auto seq = Sequence::create(blink, CallFunc::create(CC_CALLBACK_0(DropLand::remove, this)), nullptr);
	this->runAction(seq);
}