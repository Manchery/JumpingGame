#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Chapter2Level1.h"
#include "HelloScene.h"
#include "SwingLand.h"
#include "Door.h"
#include "SlidingLand.h"
using namespace CocosDenshion;

cocos2d::Scene * Chapter2Level1::createScene()
{
	return Chapter2Level1::create();
}


bool Chapter2Level1::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	chapterID = 2;

	initMap("map/chapter2Level1.tmx",Color4B::Color4B(53,73,94,255));
	coinTotal = chapterCoinTotal[2];
	commonInitAfterMap();

	return true;
}

void Chapter2Level1::messageUpdate(float dt)
{
	if (!toldHello && runningTime >= 2.0f) {
		messageDoubleLine("Dirty smelly drainage ditch,", "you can save time by diving swiftly.");
		toldHello = 1;
	}
}

void Chapter2Level1::initBackgroundMusic()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/loops/water.wav", true);
}


void Chapter2Level1::drawMap(const TMXTiledMap *tileMap)
{
	GameScene::drawMap(tileMap);
	auto dic = tileMap->getObjectGroup("game")->getObject("ShieldEnable");

	std::string type = dic.at("type").asString();
	float width = dic.at("width").asFloat();
	float height = dic.at("height").asFloat();
	float x = dic.at("x").asFloat();
	float y = dic.at("y").asFloat() + height;
	bool flippedX = (dic.at("gid").asFloat() > 2e9);
	float rotation = dic.at("rotation").asFloat();

	auto sprite = Sprite::create("map/" + type + ".png");
	sprite->setContentSize(Size(width, height));
	sprite->setAnchorPoint(Vec2::ZERO);
	sprite->setPosition(x, y);
	if (flippedX) sprite->setFlippedX(true);
	if (rotation != 0) sprite->setRotation(rotation);

	auto physicsBody = PhysicsBody::createBox(sprite->getContentSize()*0.3f, PhysicsMaterial(0.1f, 0.0f, 0.0f));

	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(COIN_M);
	physicsBody->setCollisionBitmask(HERO_M);
	physicsBody->setContactTestBitmask(0xFFFFFFFF);

	sprite->setPhysicsBody(physicsBody);
	sprite->setName("EnableShield");
	frontGroundLayer->addChild(sprite, 0);
}

bool Chapter2Level1::onContactBegin(cocos2d::PhysicsContact & contact)
{
	if (!GameScene::onContactBegin(contact))
		return false;
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA == nullptr || nodeB == nullptr) return false;

	if ((nodeA->getTag() == HERO_T && nodeB->getName() == "EnableShield") ||
		(nodeB->getTag() == HERO_T && nodeA->getName() == "EnableShield")) {
		if (!hero->getTypeUnlocked(2)) {
			gotShield = 1;
			EFFECT("equip.mp3");
			//UserDefault::getInstance()->setBoolForKey("canShot", true);
			messageDoubleLine("New-found Shield! A new Knight form is unlocked.", "Shield protects you against spikes!");
		}
		return false;
	}
	return true;
}

void Chapter2Level1::gamePass()
{
	win();
	UserDefault::getInstance()->setBoolForKey("canShield", true);
}

