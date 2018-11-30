#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Chapter1Level1.h"
#include "HelloScene.h"
#include "SwingLand.h"
#include "Door.h"
using namespace CocosDenshion;

cocos2d::Scene * Chapter1Level1::createScene()
{
	return Chapter1Level1::create();
}

bool Chapter1Level1::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	initMap("map/chapter1Level1.tmx", Color4B::Color4B(53, 73, 94, 255));
	coinTotal = chapterCoinTotal[1];
	commonInitAfterMap();

	return true;
}

void Chapter1Level1::drawMap(const TMXTiledMap *tileMap)
{
	GameScene::drawMap(tileMap);
	auto dic = tileMap->getObjectGroup("game")->getObject("ShotEnable");
	
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
	sprite->setName("EnableShot");
	frontGroundLayer->addChild(sprite, 0);
}

bool Chapter1Level1::onContactBegin(cocos2d::PhysicsContact & contact)
{
	if (!GameScene::onContactBegin(contact))
		return false;
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA == nullptr || nodeB == nullptr) return false;

	if ((nodeA->getTag() == HERO_T && nodeB->getName() == "EnableShot") ||
		(nodeB->getTag() == HERO_T && nodeA->getName() == "EnableShot")) {
		if (!UserDefault::getInstance()->getBoolForKey("canShot")) {
			gotShot = 1;
			//UserDefault::getInstance()->setBoolForKey("canShot", true);
			messageSingleLine("Sword!");
		}
		return false;
	}
	return true;
}

void Chapter1Level1::switchScene(float dt)
{
	Director::getInstance()->replaceScene(HelloScene::createScene());
}

void Chapter1Level1::gamePass()
{
	win();
	auto userData = UserDefault::getInstance();
	userData->setBoolForKey("chapter1Pass", true);
	auto record = userData->getIntegerForKey("chapter1CoinCount");
	userData->setIntegerForKey("chapter1CoinCount", std::max(record, coinCount));

	UserDefault::getInstance()->setBoolForKey("canShot", true);
}

