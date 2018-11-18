#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include "PauseScene.h"
#include "ReviveScene.h"
#include "common.h"
#include "Hero.h"
#include "Enemy.h"
#include <algorithm>

USING_NS_CC;
using namespace CocosDenshion;

#define PHYSICS_SUBSTEPS 10

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
	if (PHYSICS_DRAW_DEBUG)
		physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	physicsWorld->setSubsteps(PHYSICS_SUBSTEPS);

	auto edgeSp = Sprite::create();
	auto boundBody = PhysicsBody::createEdgeBox(mapSize, PhysicsMaterial(0.0f, 0.0f, 0.0f), 3);
	boundBody->setContactTestBitmask(0xFFFFFFFF);
	edgeSp->setPosition(mapSize.width / 2, mapSize.height / 2);
	edgeSp->setPhysicsBody(boundBody);
	edgeSp->setTag(BORDER_T);

	frontGroundLayer->addChild(edgeSp, -1);
	
	//audio
	if (AUDIO_PLAY)
		SimpleAudioEngine::getInstance()->playBackgroundMusic("parkour_sounds/spring_music.wav", true);

	//map
	initMap();
	//listener
	initListener();

	//hero
	hero = Hero::create();
	hero->setAnchorPoint(Vec2::ZERO);
	hero->setPosition(10, 100);
	hero->setName("Hero");
	hero->setTag(HERO_T);
	frontGroundLayer->addChild(hero, 1);

	auto enemy = Enemy::create();
	auto testSprite = Sprite::create("chapter2/ZigzagForest_Square.png");
	enemy->setAnchorPoint(Vec2::ZERO);
	enemy->setTrack(testSprite->getContentSize().height, 700, 1400);
	enemy->setTag(ENEMY_T);
	enemy->setName("enemy1");
	enemy->retain();
	//enemies.push_back(enemy);
	frontGroundLayer->addChild(enemy, 1);
	testSprite = NULL;

	revivePoint = hero->getPosition();
	heroDied = 0;

	this->schedule(schedule_selector(GameScene::heroUpdate));
	this->schedule(schedule_selector(GameScene::mapUpdate));

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
		sprite->setTag(LAND_T);

		sX += sprite->getContentSize().width;

		if (i == 2 || i == 3) continue; 
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
		sprite->setTag(SOFT_LAND_T);

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
		sprite->setTag(SOFT_LAND_T);

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
		sprite->setTag(SOFT_LAND_T);

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
	keyListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
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

	if (nodeA->getTag() == BULLET_T) {
		nodeA->removeFromParentAndCleanup(1);
		if (nodeB->getTag() == SOFT_LAND_T) {
			nodeB->retain();
			destroyedList.push_back(nodeB);
			nodeB->removeFromParent();
		}
		else if (nodeB->getTag() == ENEMY_T) {
			nodeB->retain();
			destroyedList.push_back(nodeB);
			nodeB->removeFromParent(); 
		}
		return false;
	}
	if (nodeB->getTag() == BULLET_T) {
		nodeB->removeFromParentAndCleanup(1);
		if (nodeA->getTag() == SOFT_LAND_T) {
			nodeA->retain();
			destroyedList.push_back(nodeA);
			nodeA->removeFromParent(); 
		}
		else if (nodeA->getTag() == ENEMY_T) {
			nodeA->retain();
			destroyedList.push_back(nodeA);
			nodeA->removeFromParent(); 
		}
		return false;
	}
	if ((nodeA->getTag() == HERO_T && nodeB->getTag() == ENEMY_T) ||
		(nodeA->getTag() == ENEMY_T && nodeB->getTag() == HERO_T)) {
		heroDied = 1;
		return false;
	}

	if (nodeB->getTag() == HERO_T) std::swap(nodeA, nodeB);
	Vec2 heroPosition = hero->getPosition();

	if (nodeB->getTag() == LAND_T || nodeB->getTag()==SOFT_LAND_T) {
		if (nodeB->getPositionY() + nodeB->getContentSize().height <= heroPosition.y + 5.0f &&
			nodeB->getPositionY() + nodeB->getContentSize().height >= heroPosition.y - 5.0f) {
			hero->resetJumpTimes();
			hero->setOnGround();
		}
	}
	else if (nodeB->getTag() == BORDER_T) {
		if (heroPosition.y <= 10.0f) {
			heroDied = 1;
			return false;
		}
	}
	return true;
}
bool GameScene::onContactEnd(const cocos2d::PhysicsContact &contact) {
	return true;
}
bool GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	//log("Key with keycode %d pressed", keyCode);
	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
		rightKeyDown = 1;
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
		leftKeyDown = 1;
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
		upKeyDown = 1;
		//log("%d %d", hero->getJumpTimes(), hero->getOnGround());
		if ((hero->getJumpTimes() < hero->getJumpLimit()) && !(hero->getJumpTimes()==0 && !hero->getOnGround())) {
			auto velcolity = hero->getPhysicsBody()->getVelocity();
			hero->getPhysicsBody()->setVelocity(Vec2(velcolity.x, 300));
			hero->addJumpTimes();
			hero->resetOnGround();
			if (AUDIO_PLAY)
				SimpleAudioEngine::getInstance()->playEffect("parkour_sounds/jump.wav", false, 1.0f, 1.0f, 1.0f);
		}
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
		downKeyDown = 1;
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
		this->gamePause();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_S) {
		hero->shot();
	}
	return true;
}
bool GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	//log("Key with keycode %d released", keyCode);
	/*if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE) {
		Director::getInstance()->replaceScene(Chapter2::createScene());
	}*/
	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
		rightKeyDown = 0; lastKey = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
		leftKeyDown = 0; lastKey = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
		upKeyDown = 0;
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
		downKeyDown = 0;
	}
	return true;
}

void GameScene::heroUpdate(float dt)
{
	if (heroDied) {
		heroDie();
		return;
	}
	auto delta = 200.0f;
	auto velcolity = hero->getPhysicsBody()->getVelocity();
	velcolity.x = 0; 
	//log("%d", Director::getInstance()->getActionManager()->getNumberOfRunningActionsInTarget(hero));
	if (rightKeyDown && leftKeyDown) {
		hero->silence();
	}
	else if (rightKeyDown) {
		velcolity.x += delta;
		hero->right();
	}
	else if (leftKeyDown) {
		velcolity.x -= delta;
		hero->left();
	}
	else {
		if (lastKey == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
			hero->rightSilence();
		}
		else if (lastKey == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
			hero->leftSilence();
		}
		else
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

void GameScene::screenShot() {

}

void GameScene::gamePause() {
	Director::getInstance()->pushScene(PauseScene::createScene());
}
void GameScene::heroDie() {
	Director::getInstance()->pushScene(ReviveScene::createScene());
	hero->setPosition(revivePoint);
	heroDied = 0;
	upKeyDown=leftKeyDown=rightKeyDown=downKeyDown=0;
	lastKey = EventKeyboard::KeyCode::KEY_NONE;
	for (auto node:destroyedList) {
		auto sprite = (Sprite*)node;
		frontGroundLayer->addChild(sprite, 1);
		if (sprite->getTag() == ENEMY_T) {
			Enemy *enemy = (Enemy*)node;
			enemy->resetPosition();
		}
	}
	destroyedList.clear();
}