#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Chapter5Level1.h"
#include "HelloScene.h"
#include "SwingLand.h"
#include "SlidingLand.h"
#include "Boss.h"
using namespace CocosDenshion;

cocos2d::Scene * Chapter5Level1::createScene()
{
	return Chapter5Level1::create();
}

void Chapter5Level1::onEnterTransitionDidFinish()
{
	//flush
	hero->getPhysicsBody()->setVelocity(Vec2::ZERO);
	upKeyDown = leftKeyDown = rightKeyDown = downKeyDown = 0;
	lastKey = EventKeyboard::KeyCode::KEY_NONE;
	heroJumped = heroBounced = 0;

	frontGroundLayer->setPosition(Vec2::ZERO);
	backGroundLayer->setPosition(Vec2::ZERO);
	
	for (int i = 0; i < trapCnt; i++) {
		auto deadTrap = frontGroundLayer->getChildByName("DeadTrap"+std::to_string(i));
		deadTrap->setPositionX(0.0f);
	}

	if (heroDied) {
		for (auto node : destroyedList) {
			auto sprite = (Sprite*)node;

			if (sprite->getTag() == COIN_T) continue; // make it easy

			frontGroundLayer->addChild(sprite, 1);
			if (sprite->getTag() == ENEMY_T) {
				Enemy *enemy = (Enemy*)node;
				enemy->resetPosition();
			}
			if (sprite->getTag() == GAME_KEY_T)
				gotGameKey = 0;
		}
		destroyedList.clear();
		heroDied = 0;
	}
}

#define SCENEVELOCITY 150.0f

bool Chapter5Level1::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	initMap("map/chapter5Level1.tmx", Color4B::Color4B(53, 73, 94, 255));
	coinTotal = chapterCoinTotal[5];
	commonInitAfterMap();

	drawBoss("map/chapter5Level1.tmx");

	/*Size visibleSize = Director::getInstance()->getVisibleSize();
	auto moveTo = MoveTo::create((visibleSize.width - mapSize.width)/SCENEVELOCITY, 
		Vec2(-visibleSize.width + mapSize.width,0));
	frontGroundLayer->runAction(moveTo);
	backGroundLayer->runAction(moveTo->clone());*/

	//logUserDefault();
	return true;
}

void Chapter5Level1::drawBoss(const std::string & tmxFile)
{
	auto tileMap = TMXTiledMap::create(tmxFile);
	auto arrObj = tileMap->getObjectGroup("boss")->getObjects();
	trapCnt = 0;
	for (auto object : arrObj)
	{
		auto dic = object.asValueMap();
		std::string type = dic.at("type").asString();
		std::string name = dic.at("name").asString();
		float width = dic.at("width").asFloat();
		float height = dic.at("height").asFloat();
		float x = dic.at("x").asFloat();
		float y = dic.at("y").asFloat() + height;

		bool flippedX = (dic.at("gid").asFloat() > 2e9);
		float rotation = dic.at("rotation").asFloat();

		if (name == "Boss") {
			auto boss = Boss::create(width,height);
			boss->setAnchorPoint(Vec2::ZERO);
			boss->setPosition(x, y);
			boss->setTag(BOSS_T);
			this->addChild(boss, 2);
		}
		else if (name == "Bound") {
			auto sprite = Sprite::create("map/earthWall.png");
			sprite->setOpacity(0);
			sprite->setContentSize(Size(width, height));
			sprite->setAnchorPoint(Vec2::ZERO);
			sprite->setPosition(x, y);
			auto physicsBody = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));

			sprite->setTag(LAND_T);

			physicsBody->setDynamic(false);
			physicsBody->setCategoryBitmask(LAND_M);
			physicsBody->setCollisionBitmask(HERO_M | ENEMY_M | BULLET_M);
			physicsBody->setContactTestBitmask(0xFFFFFFFF);

			sprite->setPhysicsBody(physicsBody);
			this->addChild(sprite, 0);
		}
		else if (name == "Trap") {
			auto sprite = Sprite::create("map/" + type + ".png");
			sprite->setContentSize(Size(width, height));
			sprite->setAnchorPoint(Vec2::ZERO);
			sprite->setPosition(x, y);
			if (flippedX)
				sprite->setFlippedX(true);
			
			sprite->setName("DeadTrap"+std::to_string(trapCnt++));
			sprite->setTag(DEAD_TRAP_T);

			auto physicsBody = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));

			physicsBody->setDynamic(false);
			physicsBody->setCategoryBitmask(LAND_M);
			physicsBody->setCollisionBitmask(HERO_M | ENEMY_M | BULLET_M);
			physicsBody->setContactTestBitmask(0xFFFFFFFF);

			physicsBody->setVelocity(Vec2(SCENEVELOCITY, 0));
			sprite->setPhysicsBody(physicsBody);
			if (rotation != 0)
				sprite->setRotation(rotation);

			frontGroundLayer->addChild(sprite, 1);
		}
	}
}

void Chapter5Level1::switchScene(float dt)
{
	Director::getInstance()->replaceScene(HelloScene::createScene());
}

void Chapter5Level1::gamePass()
{
	win();
	auto userData = UserDefault::getInstance();
	userData->setBoolForKey("chapter5Pass", true);
	auto record = userData->getIntegerForKey("chapter5CoinCount");
	userData->setIntegerForKey("chapter5CoinCount", std::max(record, coinCount));
}

void Chapter5Level1::mapUpdate(float dt){
	frontGroundLayer->setPosition(frontGroundLayer->getPositionX() - SCENEVELOCITY * dt, 
		frontGroundLayer->getPositionY());
	backGroundLayer->setPosition(backGroundLayer->getPositionX() - SCENEVELOCITY * dt,
		backGroundLayer->getPositionY());
}

bool Chapter5Level1::onContactBegin(cocos2d::PhysicsContact & contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA == nullptr || nodeB == nullptr) return false;

	if (nodeB->getTag()==BULLET_T) std::swap(nodeA, nodeB);
	
	if (nodeA->getTag() == BULLET_T && nodeB->getTag() == BOSS_T) {
		nodeA->removeFromParentAndCleanup(1);
		auto boss = (Boss*)nodeB;
		if (boss->damaged()) {
			gamePass();
		}
		return false;
	}

	if (isHero(nodeB)) std::swap(nodeA, nodeB);

	if (isHero(nodeA)) {
		if (nodeB->getTag() == DEAD_TRAP_T || nodeB->getTag()==BOSS_T) {
			heroDied = 1;
			return false;
		}
	}

	if (!GameScene::onContactBegin(contact))
		return false;

	return true;
}
