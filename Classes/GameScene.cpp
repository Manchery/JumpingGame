#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include <algorithm>

USING_NS_CC;
using namespace CocosDenshion;

Scene* GameScene::createScene()
{
	//return GameScene::createWithPhysics();
	return GameScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	mapSize = Size(visibleSize.width * 2, visibleSize.height*1.5);

	frontGroundLayer = Layer::create();
	frontGroundLayer->setContentSize(mapSize);
	this->addChild(frontGroundLayer, 0);

	backGroundLayer = Layer::create();
	backGroundLayer->setContentSize(mapSize);
	this->addChild(backGroundLayer, -1);

	//physics
	auto physicsWorld = this->getPhysicsWorld();
	physicsWorld->setGravity(Vec2(0, -500));
	//physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	physicsWorld->setSubsteps(20);

	auto edgeSp = Sprite::create();
	auto boundBody = PhysicsBody::createEdgeBox(mapSize, PhysicsMaterial(0.0f, 0.0f, 0.0f), 3);
	edgeSp->setPosition(mapSize.width / 2, mapSize.height / 2);
	edgeSp->setPhysicsBody(boundBody);
	frontGroundLayer->addChild(edgeSp, -1);
	
	//audio
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("parkour_sounds/spring_music.wav", true);

	//map
	initMap();
	//listener
	initListener();

	//hero
	hero = Hero::create();
	hero->setAnchorPoint(Vec2(0, 0));
	hero->setPosition(10, 100);
	hero->setName("Hero");
	hero->setTag(1);
	frontGroundLayer->addChild(hero, 1);

	this->schedule(schedule_selector(GameScene::heroUpdate), 1.0f / 60);
	this->schedule(schedule_selector(GameScene::mapUpdate), 1.0f / 60);

	return true;
}
void GameScene::initMap() {
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

	// left side blocks
	sX = mapSize.width / 4 - testSprite->getContentSize().width;
	//sY = playingSize.height / 2 - testSprite->getContentSize().height * 2;
	sY = mapSize.height - playingSize.height;

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
	sX = (mapSize.width / 2 + mapSize.width / 4) - testSprite->getContentSize().width;
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

		nodeItems->addChild(sprite, 1);
	}

	// center blocks
	sX = mapSize.width / 2 - testSprite->getContentSize().width;
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
	frontGroundLayer->addChild(nodeItems, 1);
}
void GameScene::initListener() {
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
				hero->setJumpTimes(hero->getJumpTimes() + 1);

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

void GameScene::menuExitCallback(Ref* pSender)
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
bool GameScene::onContactBegin(const cocos2d::PhysicsContact &contact) {
	//log("contact begin");
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	//log("%d %d", nodeA->getTag(), nodeB->getTag());
	if (nodeB->getTag() == 1) std::swap(nodeA, nodeB);
	//log("%f %f", nodeB->getPositionY() + nodeB->getContentSize().height, nodeA->getPositionY());
	auto eps = 1.0f;
	if (nodeB->getPositionY() + nodeB->getContentSize().height <= nodeA->getPositionY() + eps) {
		hero->setJumpTimes(0);
	}
	return true;
}
bool GameScene::onContactEnd(const cocos2d::PhysicsContact &contact) {
	//log("contact end");
	return true;
}

void GameScene::heroUpdate(float dt)
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
		hero->silence();
	}
	hero->getPhysicsBody()->setVelocity(velcolity);
}
void GameScene::mapUpdate(float dt) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto heroBound = Rect::Rect(visibleSize.width / 3, visibleSize.height/3, visibleSize.width / 3, visibleSize.height / 3);
	auto heroPosition = getHeroGlobalPosition();
	if (!heroBound.containsPoint(heroPosition)) {
		//log("out! %f %f",heroPosition.x,heroPosition.y);
		Vec2 delta=Vec2(0,0);
		if (heroPosition.x < heroBound.getMinX())
			delta.x = (heroBound.getMinX() - heroPosition.x);
		else if (heroPosition.x > heroBound.getMaxX())
			delta.x = -(heroPosition.x - heroBound.getMaxX());

		if (heroPosition.y < heroBound.getMinY())
			delta.y = (heroBound.getMinY() - heroPosition.y);
		else if (heroPosition.y > heroBound.getMaxY())
			delta.y = -(heroPosition.y - heroBound.getMaxY());

		//log("%f %f", delta.x, delta.y);

		auto mapBound = Rect::Rect(visibleSize.width-mapSize.width, visibleSize.height - mapSize.height, 
			mapSize.width - visibleSize.width, mapSize.height - visibleSize.height);
		auto layerPosition = frontGroundLayer->getPosition();
		layerPosition.x = std::max(std::min(mapBound.getMaxX(), layerPosition.x + delta.x), mapBound.getMinX());
		layerPosition.y = std::max(std::min(mapBound.getMaxY(), layerPosition.y + delta.y), mapBound.getMinY());
		frontGroundLayer->setPosition(layerPosition);
	}
}

Vec2 GameScene::getHeroGlobalPosition() {
	return hero->getPosition() + hero->getParent()->getPosition();
}
