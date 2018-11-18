#include "TileTestScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include <algorithm>

USING_NS_CC;
using namespace CocosDenshion;

Scene* TileTestScene::createScene()
{
	//return GameScene::createWithPhysics();
	return TileTestScene::create();
}

void TileTestScene::initMap() {
	Size mapSize = frontGroundLayer->getContentSize();

	//background 
	auto backGround = Sprite::create("parkour_images/about.jpg");
	backGround->setContentSize(mapSize);
	backGround->setAnchorPoint(Vec2::ZERO);
	backGround->setPosition(Vec2::ZERO);
	backGroundLayer->addChild(backGround, -1);


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// create a node to hold non-sprites.
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	auto nodeItems = Node::create();
	nodeItems->setName("nodeItems");

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// create a path/walkway
	// depending upon how large the screen is we need to decide how many blocks to lay down.
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	auto testSprite = Sprite::create("chapter2/ZigzagForest_Square.png");

	int howMany = std::ceil(mapSize.width / testSprite->getContentSize().width);

	int sX = 0; // act as a counter for setPosition x coordinate.
	int sY = 0; // act as a counter for setPosition y coordinate.

	auto playingSize = Size(mapSize.width, mapSize.height - testSprite->getContentSize().height);

	for (int i = 0; i < howMany; i++)
	{
		auto sprite = Sprite::create("chapter2/ZigzagForest_Square.png");
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(sX, sY);

		auto physicsBody = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setCategoryBitmask(0x04);    // 0100
		physicsBody->setCollisionBitmask(0x01);   // 0001
		sprite->setPhysicsBody(physicsBody);
		sprite->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
		sprite->setTag(3);

		sX += sprite->getContentSize().width;

		nodeItems->addChild(sprite, -1);
	}

	testSprite = NULL;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// create a few blocks as obstables
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	testSprite = Sprite::create("chapter2/ZigzagGrass_Mud_Round.png");

	//// left side blocks
	//sX = mapSize.width / 4 - testSprite->getContentSize().width;
	////sY = playingSize.height / 2 - testSprite->getContentSize().height * 2;
	//sY = mapSize.height - playingSize.height+100;
	//for (int i = 0; i < 3; i++)
	//{

	//	auto sprite = Sprite::create("chapter2/ZigzagGrass_Mud_Round.png");
	//	sprite->setAnchorPoint(Vec2(0, 0));
	//	sprite->setPosition(sX, sY);

	//	auto physicsBody = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
	//	physicsBody->setDynamic(false);
	//	physicsBody->setCategoryBitmask(0x02);    // 0010
	//	physicsBody->setCollisionBitmask(0x01);   // 0001
	//	sprite->setPhysicsBody(physicsBody);
	//	sprite->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
	//	sprite->setTag(2);

	//	sX += sprite->getContentSize().width;

	//	if (i == 1)
	//	{
	//		sprite->setName("middleBlock1");
	//	}

	//	nodeItems->addChild(sprite, 1);
	//}

	//xml
	auto tileMap = TMXTiledMap::create("untitled.tmx");
	auto objects = tileMap->getObjectGroup("object");
	auto arrObj = objects->getObjects();
	for (auto object : arrObj)
	{
		float x = object.asValueMap().at("x").asFloat();
		float y = object.asValueMap().at("y").asFloat();
		//log("%f %f", x, y);

		auto sprite = Sprite::create("parkour_images/block_spring.png");
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(x, y);

		auto physicsBody = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setCategoryBitmask(0x02);    // 0010
		physicsBody->setCollisionBitmask(0x01);   // 0001
		sprite->setPhysicsBody(physicsBody);
		sprite->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
		sprite->setTag(2);

		nodeItems->addChild(sprite, 1);
	}


	frontGroundLayer->addChild(nodeItems, 1);
}
void TileTestScene::initListener() {
	//contactlistener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(GameScene::onContactEnd, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	//keylistener
	auto keyListener = EventListenerKeyboard::create();

	keyListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		//log("Key with keycode %d pressed", keyCode);
		if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
			rightKeyDown = 1;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
			leftKeyDown = 1;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
			upKeyDown = 1;
			if (hero->getJumpTimes() < hero->getJumpLimit()) {
				auto velcolity = hero->getPhysicsBody()->getVelocity();
				hero->getPhysicsBody()->setVelocity(Vec2(velcolity.x, 300));
				hero->addJumpTimes();

				//audio->playEffect("parkour_sounds/jump.wav", false, 1.0f, 1.0f, 1.0f);
			}
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
			downKeyDown = 1;
		}
	};

	keyListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		//log("Key with keycode %d released", keyCode);
		/*if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE) {
			Director::getInstance()->replaceScene(Chapter2::createScene());
		}*/
		if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
			rightKeyDown = 0;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
			leftKeyDown = 0;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
			upKeyDown = 0;
		}
		if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
			downKeyDown = 0;
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
}
