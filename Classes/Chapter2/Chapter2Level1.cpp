#include "Chapter2Level1.h"
#include "Chapter2.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include <algorithm>

USING_NS_CC;
using namespace CocosDenshion;

Scene* Chapter2Level1::createScene()
{
	//return Chapter2Level1::createWithPhysics();
	return Chapter2Level1::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Chapter2Level1::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::initWithPhysics())
	{
		return false;
	}
	auto physicsWorld = this->getPhysicsWorld();
	physicsWorld->setGravity(Vec2(0, -500));
	physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	physicsWorld->setSubsteps(20);
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//background 
	auto backGround = Sprite::create("parkour_images/about.jpg");
	backGround->setContentSize(visibleSize);
	backGround->setAnchorPoint(Vec2::ZERO);
	backGround->setPosition(Vec2::ZERO);
	this->addChild(backGround, -1);

	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("parkour_sounds/spring_music.wav", true);

	//edgebox
	auto edgeSp = Sprite::create();
	auto boundBody = PhysicsBody::createEdgeBox(visibleSize, PhysicsMaterial(0.0f, 0.0f, 0.0f), 3);
	edgeSp->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	edgeSp->setPhysicsBody(boundBody);
	this->addChild(edgeSp, -1);

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

	int howMany = std::ceil(visibleSize.width / testSprite->getContentSize().width);

	int sX = 0; // act as a counter for setPosition x coordinate.
	int sY = 0; // act as a counter for setPosition y coordinate.

	auto playingSize = Size(visibleSize.width, visibleSize.height - testSprite->getContentSize().height);

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

	// left side blocks
	sX = visibleSize.width / 4 - testSprite->getContentSize().width;
	//sY = playingSize.height / 2 - testSprite->getContentSize().height * 2;
	sY = visibleSize.height - playingSize.height;

	for (int i = 0; i < 3; i++)
	{

		auto sprite = Sprite::create("chapter2/ZigzagGrass_Mud_Round.png");
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(sX, sY);

		auto physicsBody = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setCategoryBitmask(0x02);    // 0010
		physicsBody->setCollisionBitmask(0x01);   // 0001
		sprite->setPhysicsBody(physicsBody);
		sprite->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
		sprite->setTag(2);

		sX += sprite->getContentSize().width;

		if (i == 1)
		{
			sprite->setName("middleBlock1");
		}

		nodeItems->addChild(sprite, 1);
	}

	// right side blocks
	sX = (visibleSize.width / 2 + visibleSize.width / 4) - testSprite->getContentSize().width;
	sY = playingSize.height / 2 - testSprite->getContentSize().height * 2;

	for (int i = 0; i < 3; i++)
	{

		auto sprite = Sprite::create("chapter2/ZigzagGrass_Mud_Round.png");
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(sX, sY);

		auto physicsBody = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setCategoryBitmask(0x02);    // 0010
		physicsBody->setCollisionBitmask(0x01);   // 0001
		sprite->setPhysicsBody(physicsBody);
		sprite->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
		sprite->setTag(2);

		sX += sprite->getContentSize().width;

		if (i == 1)
		{
			sprite->setName("middleBlock2");
		}

		this->addChild(sprite, 1);
	}

	// center blocks
	sX = visibleSize.width / 2 - testSprite->getContentSize().width;
	sY = (playingSize.height / 2 + playingSize.height / 4) - testSprite->getContentSize().height * 2;

	for (int i = 0; i < 3; i++)
	{
		auto sprite = Sprite::create("chapter2/ZigzagGrass_Mud_Round.png");
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(sX, sY); 
		
		auto physicsBody = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setCategoryBitmask(0x02);    // 0010
		physicsBody->setCollisionBitmask(0x01);   // 0001
		sprite->setPhysicsBody(physicsBody);
		sprite->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
		sprite->setTag(2);

		sX += sprite->getContentSize().width;

		nodeItems->addChild(sprite, 1);
	}

	testSprite = NULL;

	this->addChild(nodeItems, 1);

	hero = Hero::create();
	hero->setAnchorPoint(Vec2(0, 0));
	hero->setPosition(10, 100);
	hero->setName("Hero");
	hero->setTag(1);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Chapter2Level1::onContactBegin, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(Chapter2Level1::onContactEnd, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
	this->addChild(hero, 1);

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
				hero->getPhysicsBody()->setVelocity(Vec2(velcolity.x,300));
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
		if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE) {
			Director::getInstance()->replaceScene(Chapter2::createScene());
		}
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

	this->schedule(schedule_selector(Chapter2Level1::heroUpdate), 1.0f/30);
	return true;
}


void Chapter2Level1::menuExitCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}
bool Chapter2Level1::onContactBegin(const cocos2d::PhysicsContact &contact) {
	//log("contact begin");
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	log("%d %d", nodeA->getTag(), nodeB->getTag());
	if (nodeB->getTag() == 1) std::swap(nodeA, nodeB);
	if (nodeB->getPositionY() + nodeB->getContentSize().height <= nodeA->getPositionY()) {
		hero->resetJumpTimes();
	}
	return true;
}
bool Chapter2Level1::onContactEnd(const cocos2d::PhysicsContact &contact) {
	//log("contact end");
	return true;
}

void Chapter2Level1::heroUpdate(float dt)
{
	auto delta = 200.0f;
	auto velcolity = hero->getPhysicsBody()->getVelocity();
	velcolity.x = 0; 
	//log("%d", Director::getInstance()->getActionManager()->getNumberOfRunningActionsInTarget(hero));
	if (rightKeyDown^leftKeyDown) {
		if (rightKeyDown) {
			velcolity.x += delta;
			hero->right();
		}
		else if (leftKeyDown) {
			velcolity.x -= delta;
			hero->left();
		}
	}else if (upKeyDown) {
		hero->jump();
	}else {
		hero->rightSilence();
	}
	hero->getPhysicsBody()->setVelocity(velcolity);
}
