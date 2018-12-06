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

	chapterID = 1;

	initMap("map/chapter1Level1.tmx", Color4B::Color4B(53, 73, 94, 255));
	coinTotal = chapterCoinTotal[1];
	commonInitAfterMap();

	gotSwordTime = 1e9; gotDoorOpen = false;

	return true;
}

void Chapter1Level1::messageUpdate(float dt)
{
	if (runningTime - gotSwordTime > 80.0f && !gotDoorOpen && !toldDoor) {
		messageSingleLine("Maybe you can trigger some gear with a dart of sword.");
		toldDoor = true;
	}
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
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA == nullptr || nodeB == nullptr) return false;

	if ((nodeA->getTag() == HERO_T && nodeB->getName() == "EnableShot") ||
		(nodeB->getTag() == HERO_T && nodeA->getName() == "EnableShot")) {
		if (!hero->getTypeUnlocked(1)) {
			gotShot = 1;
			EFFECT("equip.mp3");
			//UserDefault::getInstance()->setBoolForKey("canShot", true);
			messageDoubleLine("New-found Sword! A new Knight form is unlocked.","Be sure to experiment a dart of sword!");

			gotSwordTime = runningTime;
		}
		return false;
	}
	if ((nodeA->getTag() == DOOR_KEY_T && nodeB->getTag()==BULLET_T) || 
		(nodeB->getTag() == DOOR_KEY_T && nodeA->getTag() == BULLET_T)) {
		gotDoorOpen = true;
	}
	if (!GameScene::onContactBegin(contact))
		return false;
	return true;
}

void Chapter1Level1::gamePass()
{
	win();
	UserDefault::getInstance()->setBoolForKey("canShot", true);
}

