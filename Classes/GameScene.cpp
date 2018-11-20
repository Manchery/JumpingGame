#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include "PauseScene.h"
#include "ReviveScene.h"
#include "common.h"
#include "SlidingLand.h"
#include "Hero.h"
#include "Enemy.h"
#include "DropLand.h"
#include "SwingLand.h"
#include "SlidingTrap.h"
#include <algorithm>
using namespace CocosDenshion;

#define PHYSICS_SUBSTEPS 10

void GameScene::onEnterTransitionDidFinish()
{
	//flush
	hero->getPhysicsBody()->setVelocity(Vec2::ZERO);
	upKeyDown = leftKeyDown = rightKeyDown = downKeyDown = 0;
	lastKey = EventKeyboard::KeyCode::KEY_NONE;
	for (auto node : destroyedList) {
		auto sprite = (Sprite*)node;
		frontGroundLayer->addChild(sprite, 1);
		if (sprite->getTag() == ENEMY_T) {
			Enemy *enemy = (Enemy*)node;
			enemy->resetPosition();
		}
	}
	destroyedList.clear();
	//add joint pin	

	auto sprite = frontGroundLayer->getChildByName("jointtest");
	if (sprite != nullptr) {
		this->getPhysicsWorld()->removeJoint(((SwingLand*)sprite)->pin);
		sprite->removeFromParentAndCleanup(1);
	}
	auto swingLand = SwingLand::create("parkour_images/aboutA.png");
	//swingLand->setAnchorPoint(Vec2::ZERO);
	swingLand->setPosition(Vec2(300, 300));
	swingLand->swing();
	swingLand->setName("jointtest");
	swingLand->setTag(SWING_LAND_T);

	frontGroundLayer->addChild(swingLand);
}

Scene* GameScene::createScene()
{
	//return GameScene::createWithPhysics();
	return GameScene::create();
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
	boundBody->setCategoryBitmask(LAND_M);
	boundBody->setCollisionBitmask(HERO_M | ENEMY_M | BULLET_M);
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
	heroDied = heroJumped= 0;

	this->schedule(schedule_selector(GameScene::heroUpdate));
	this->schedule(schedule_selector(GameScene::mapUpdate));
	this->schedule(schedule_selector(GameScene::regenerateUpdate));

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
		if (i==4 || i==5) {
			auto slidingTrap = SlidingTrap::create("chapter2/Dark_ZigzagForest_Square.png");
			slidingTrap->setAnchorPoint(Vec2(0, 0));
			slidingTrap->setTrack(sX, sY + testSprite->getContentSize().height-slidingTrap->getContentSize().height-5.0f,
				sY + testSprite->getContentSize().height);
			slidingTrap->setTag(SLIDING_TRAP_T);
			frontGroundLayer->addChild(slidingTrap, -1);
		}
		auto sprite = Sprite::create("chapter2/ZigzagForest_Square.png");
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(sX, sY);
		sprite->setTag(LAND_T);

		if (i == howMany - 2 || i == howMany - 3) {
			sprite = Sprite::create("chapter2/Dark_ZigzagForest_Square.png");
			sprite->setAnchorPoint(Vec2(0, 0));
			sprite->setPosition(sX, sY);
			sprite->setTag(TRAP_T);
		}
		
		auto physicsBody = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setCategoryBitmask(LAND_M);
		physicsBody->setCollisionBitmask(HERO_M | ENEMY_M | BULLET_M);
		physicsBody->setContactTestBitmask(0xFFFFFFFF);
		sprite->setPhysicsBody(physicsBody);

		sX += sprite->getContentSize().width;

		if (i == 2 || i == 3) continue; 
		nodeItems->addChild(sprite, -1);
	}

	testSprite = NULL;


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// create a few blocks as obstables
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	testSprite = Sprite::create("chapter2/ZigzagGrass_Mud_Round.png");

	auto slidingLand = SlidingLand::create("parkour_images/block_spring.png");
	slidingLand->setAnchorPoint(Vec2::ZERO);
	slidingLand->setTrack(mapSize.width / 4 - testSprite->getContentSize().width+100, 160,
		mapSize.width / 2 - testSprite->getContentSize().width - 100,
		(playingSize.height / 2 + playingSize.height / 4) - testSprite->getContentSize().height * 2);
	slidingLand->setTag(SLIDING_LAND_T);
	frontGroundLayer->addChild(slidingLand, 1);

	// left side blocks
	sX = mapSize.width / 4 - testSprite->getContentSize().width;
	//sY = playingSize.height / 2 - testSprite->getContentSize().height * 2;
	sY = mapSize.height - playingSize.height;

	for (int i = 0; i < 3; i++)
	{

		auto sprite = DropLand::create("parkour_images/block_winter.png");
		sprite->setAnchorPoint(Vec2(0, 0));
		sprite->setPosition(sX, sY);

		auto physicsBody = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setCategoryBitmask(LAND_M);
		physicsBody->setCollisionBitmask(HERO_M | ENEMY_M | BULLET_M);
		sprite->setPhysicsBody(physicsBody);
		sprite->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
		sprite->setTag(DROP_LAND_T);

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
		physicsBody->setCategoryBitmask(LAND_M);
		physicsBody->setCollisionBitmask(HERO_M | ENEMY_M | BULLET_M);
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
		physicsBody->setCategoryBitmask(LAND_M);
		physicsBody->setCollisionBitmask(HERO_M | ENEMY_M | BULLET_M);
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
	contactListener->onContactPreSolve = CC_CALLBACK_2(GameScene::onContactPreSolve, this);
	contactListener->onContactPostSolve = CC_CALLBACK_2(GameScene::onContactPostSolve, this);
	contactListener->onContactSeparate = CC_CALLBACK_1(GameScene::onContactEnd, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	//keylistener
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
}

bool GameScene::onContactBegin(cocos2d::PhysicsContact &contact) {
	//log("contact begin");
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	const Vec2 contactPoint=contact.getContactData()->points[0];

	if (nodeA->getTag() == SLIDING_TRAP_T || nodeB->getTag() == SLIDING_TRAP_T) {
		log("contact");
	}

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
	
	if (isHero(nodeB)) std::swap(nodeA, nodeB);

	if (isHero(nodeA)) {
		if (nodeB->getTag() == ENEMY_T || 
			(nodeB->getTag() == BORDER_T && contactPoint.y <= 10.0f)||
			(nodeB->getTag()==TRAP_T && touchUpSurface(hero,nodeB)) ||
			(nodeB->getTag() == SLIDING_TRAP_T && touchUpSurface(hero, nodeB))){
			heroDied = 1;
			return false;
		}
	}

	bool *heroSetOnGround = new bool(false);
	contact.setData(heroSetOnGround);
	return true;
}
bool GameScene::onContactPreSolve(PhysicsContact & contact, PhysicsContactPreSolve & solve)
{
	//log("pre solve");
	return true;
}
bool GameScene::onContactPostSolve(PhysicsContact & contact, const PhysicsContactPostSolve & solve)
{
	//log("post solve");
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	//const Vec2 contactPoint = contact.getContactData()->points[0];
	if (isHero(nodeB)) std::swap(nodeA, nodeB);
	if (isHero(nodeA)) {
		bool *heroSetOnGround = (bool*)contact.getData();
		//log("%f %f", nodeB->getPositionY() + nodeB->getContentSize().height, hero->getPositionY());
		if (isLand(nodeB)) {
			if (!(*heroSetOnGround)) {
				if (nodeB->getTag()==SWING_LAND_T || touchUpSurface(hero,nodeB)) {
					hero->resetJumpTimes();
					hero->setOnGround();
					*heroSetOnGround = true;
					//log("on ground");
					if (nodeB->getTag() == SLIDING_LAND_T) {
						hero->setSlidingGround((Sprite*)nodeB);
					}
					if (nodeB->getTag() == DROP_LAND_T) {
						auto dropLand = (DropLand*)nodeB;
						dropLand->drop();
					}
				}
			}
		}
	}
	return true;
}
bool GameScene::onContactEnd(cocos2d::PhysicsContact &contact) {
	if (contact.getData() == nullptr) 
		return true;
	if (*((bool*)contact.getData())){
		auto nodeA = contact.getShapeA()->getBody()->getNode();
		auto nodeB = contact.getShapeB()->getBody()->getNode();
		if (isHero(nodeB)) std::swap(nodeA, nodeB);
		if (isHero(nodeA) && isLand(nodeB)) {
			hero->resetOnGround();
			if (nodeB->getTag() == SLIDING_LAND_T) {
				hero->setSlidingGround(nullptr);
			}
		}
		delete contact.getData();
	}
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
		if ((hero->getJumpTimes() < hero->getJumpLimit()) && !(hero->getJumpTimes()==0 && !(hero->getOnGround()>0))) {
			heroJump();
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
	//died
	if (heroDied) {
		heroDie();
		return;
	}
	//heroTexture
	if (rightKeyDown && leftKeyDown) { hero->silence(); }
	else if (rightKeyDown) { hero->right(); }
	else if (leftKeyDown) { hero->left(); }
	else {
		if (lastKey == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) { hero->rightSilence(); }
		else if (lastKey == EventKeyboard::KeyCode::KEY_LEFT_ARROW) { hero->leftSilence(); }
		else hero->silence();
	}

	auto delta = 200.0f*(1.0f-hero->getPhysicsBody()->getLinearDamping());
	auto velcolity = hero->getPhysicsBody()->getVelocity(); 
	
	//velocityX
	velcolity.x = 0; 
	if (rightKeyDown) velcolity.x += delta;
	if (leftKeyDown) velcolity.x -= delta;
	if (hero->getSlidingGround() != nullptr) {
		auto slidingVel = hero->getSlidingGround()->getPhysicsBody()->getVelocity();
		velcolity.x += slidingVel.x;
	}

	//velocityY
	if (hero->getSlidingGround() != nullptr) {
		auto slidingVel = hero->getSlidingGround()->getPhysicsBody()->getVelocity();
		velcolity.y = slidingVel.y;
	}
	if (heroJumped) {
		velcolity.y = 300.0f*(1.0f - hero->getPhysicsBody()->getLinearDamping());
		heroJumped = 0;
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

void GameScene::regenerateUpdate(float dt)
{
	for (auto itor = regenList.begin(); itor != regenList.end(); ) {
		auto heroRect=hero->getBoundingBox(),nodeRect= itor->first->getBoundingBox();
		itor->second -= dt;
		if (heroRect.intersectsRect(nodeRect)) {
			itor->second = 5.0f;
			//log("cant regen");
		}
		if (itor->second <= 0) {
			frontGroundLayer->addChild(itor->first, 1);
			itor=regenList.erase(itor);
		}
		else itor++;
	}
}

void GameScene::addRenerate(Node * node){
	node->removeFromParent();
	regenList.push_back(std::make_pair(node, 5.0f));
}

Vec2 GameScene::getHeroGlobalPosition() {
	return hero->getPosition() + hero->getParent()->getPosition();
}

void GameScene::gamePause() {
	Director::getInstance()->pushScene(PauseScene::createScene());
}
void GameScene::heroDie() {
	Director::getInstance()->pushScene(ReviveScene::createScene());
	hero->setPosition(revivePoint);
	heroDied = 0;
}
void GameScene::heroJump() {
	heroJumped = 1;
	hero->addJumpTimes();
	if (AUDIO_PLAY)
		SimpleAudioEngine::getInstance()->playEffect("parkour_sounds/jump.wav", false, 1.0f, 1.0f, 1.0f);
}