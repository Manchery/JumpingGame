#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "HelloScene.h"
#include "PauseScene.h"
#include "ReviveScene.h"
#include "common.h"
#include "SlidingLand.h"
#include "Hero.h"
#include "Enemy.h"
#include "DropLand.h"
#include "SwingLand.h"
#include "SlidingTrap.h"
#include "FollowEnemy.h"
#include "Door.h"
#include <sstream>
#include <algorithm>
using namespace CocosDenshion;

#define PHYSICS_SUBSTEPS 10

void GameScene::onEnterTransitionDidFinish()
{
	//flush
	hero->getPhysicsBody()->setVelocity(Vec2::ZERO);
	upKeyDown = leftKeyDown = rightKeyDown = downKeyDown = 0;
	lastKey = EventKeyboard::KeyCode::KEY_NONE;

	if (heroDied) {
		for (auto node : destroyedList) {
			auto sprite = (Sprite*)node;
			frontGroundLayer->addChild(sprite, 1);
			if (sprite->getTag() == ENEMY_T) {
				Enemy *enemy = (Enemy*)node;
				enemy->resetPosition();
			}
			if (sprite->getTag() == COIN_T)
				coinCount--;
			if (sprite->getTag() == GAME_KEY_T)
				gotGameKey=0;
		}
		destroyedList.clear();
		heroDied = 0;
	}

	for (auto swingLand : swingLands) {
		swingLand->swing(this);
	}
}

Scene* GameScene::createScene()
{
	//return GameScene::createWithPhysics();
	return GameScene::create();
}

void GameScene::drawBackGround(ValueVector &arrObj, int zOrder = 0) {
	for (auto object : arrObj)
	{
		auto dic = object.asValueMap();
		std::string type = dic.at("type").asString();
		float width = dic.at("width").asFloat();
		float height = dic.at("height").asFloat();
		float x = dic.at("x").asFloat();
		float y = dic.at("y").asFloat() + height;
		float rotation = dic.at("rotation").asFloat();
		bool flippedX = (dic.at("gid").asFloat() > 2e9);

		if (type != "") {
			auto sprite = Sprite::create("map/" + type + ".png");
			sprite->setContentSize(Size(width, height));
			sprite->setAnchorPoint(Vec2::ZERO);
			sprite->setPosition(x, y);
			if (rotation != 0)
				sprite->setRotation(rotation);
			if (flippedX)
				sprite->setFlippedX(true);
			if (type == "shadow")
				sprite->setOpacity(64);
			backGroundLayer->addChild(sprite, zOrder);
		}
	}
}
void GameScene::drawMap(const TMXTiledMap *tileMap) {
	auto arrObj = tileMap->getObjectGroup("game")->getObjects();
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

		if (type == "") continue;

		if (name == "Hero") {
			hero = Hero::create(); hero->retain();
			hero->setAnchorPoint(Vec2::ZERO);
			hero->setPosition(x, y);
			hero->setName("Hero");
			hero->setTag(HERO_T);
			frontGroundLayer->addChild(hero, 1);
		}
		else if (name == "Enemy") {
			float minX = dic.at("minX").asFloat(), maxX = dic.at("maxX").asFloat();
			auto enemy = Enemy::create();
			enemy->setAnchorPoint(Vec2::ZERO);
			enemy->setTrack(y, minX, maxX);
			enemy->setTag(ENEMY_T);
			//enemy->setName("enemy1");
			enemy->retain();
			//enemies.push_back(enemy);
			frontGroundLayer->addChild(enemy, 1);
		}
		else if (name == "SlidingTrap") {
			auto slidingTrap = SlidingTrap::create("map/" + type + ".png");
			slidingTrap->setContentSize(Size(width, height));
			slidingTrap->setAnchorPoint(Vec2(0, 0));
			slidingTrap->setTrack(x, y - slidingTrap->getContentSize().height - 10.0f, y);

			slidingTrap->setTag(SLIDING_TRAP_T);
			frontGroundLayer->addChild(slidingTrap, -1);
		}
		else if (name == "SlidingLand") {
			auto slidingLand = SlidingLand::create("map/" + type + ".png");
			slidingLand->setContentSize(Size(width, height));
			slidingLand->setAnchorPoint(Vec2::ZERO);
			float startX= dic.at("startX").asFloat(),startY= mapSize.height-dic.at("startY").asFloat();
			float endX = dic.at("endX").asFloat(), endY = mapSize.height - dic.at("endY").asFloat();
			slidingLand->setTrack(startX,startY,endX,endY);
			slidingLand->setTag(SLIDING_LAND_T);
			frontGroundLayer->addChild(slidingLand, 1);
		}
		else if (name == "SwingLand") {
			auto swingLand = SwingLand::create("map/" + type + ".png");
			//swingLand->setContentSize(Size(width, height));
			swingLand->setPosition(Vec2(x+swingLand->getContentSize().width/2, 
				y+swingLand->getContentSize().height/2));
			swingLand->setTag(SWING_LAND_T);
			swingLand->retain();

			swingLands.push_back(swingLand);
			frontGroundLayer->addChild(swingLand);
		}
		else {
			auto sprite = Sprite::create("map/" + type + ".png");
			sprite->setContentSize(Size(width, height));
			sprite->setAnchorPoint(Vec2::ZERO);
			sprite->setPosition(x, y);
			if (flippedX)
				sprite->setFlippedX(true);
			auto physicsBody = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));

			if (name == "Land") {
				sprite->setTag(LAND_T);

				physicsBody->setDynamic(false);
				physicsBody->setCategoryBitmask(LAND_M);
				physicsBody->setCollisionBitmask(HERO_M | ENEMY_M | BULLET_M);
				physicsBody->setContactTestBitmask(0xFFFFFFFF);

				sprite->setPhysicsBody(physicsBody);
				if (rotation != 0)
					sprite->setRotation(rotation);

				if (dic.find("revivePoint")!=dic.end() && dic.at("revivePoint").asBool())
					sprite->setName("RevivePoint");

				frontGroundLayer->addChild(sprite, 0);
			}
			else if (name == "Trap") {
				sprite->setTag(TRAP_T);

				physicsBody->setDynamic(false);
				physicsBody->setCategoryBitmask(LAND_M);
				physicsBody->setCollisionBitmask(HERO_M | ENEMY_M | BULLET_M);
				physicsBody->setContactTestBitmask(0xFFFFFFFF);

				sprite->setPhysicsBody(physicsBody);
				frontGroundLayer->addChild(sprite, 0);
			}
			else if (name == "Coin") {
				sprite->setTag(COIN_T);

				physicsBody->setDynamic(false);
				physicsBody->setCategoryBitmask(COIN_M);
				physicsBody->setCollisionBitmask(HERO_M);
				physicsBody->setContactTestBitmask(0xFFFFFFFF);

				sprite->setPhysicsBody(physicsBody);
				frontGroundLayer->addChild(sprite, 0);
			}
			else if (name == "GameKey") {
				sprite->setTag(GAME_KEY_T);

				physicsBody->setDynamic(false);
				physicsBody->setCategoryBitmask(GAME_KEY_M);
				physicsBody->setCollisionBitmask(HERO_M);
				physicsBody->setContactTestBitmask(0xFFFFFFFF);

				sprite->setPhysicsBody(physicsBody);
				frontGroundLayer->addChild(sprite, 0);
			}
			else if (name == "Exit") {
				sprite->setTag(EXIT_T);
				sprite->setName("Exit");

				auto physicsBody = PhysicsBody::createBox(sprite->getContentSize()*0.3f, PhysicsMaterial(0.1f, 0.0f, 0.0f));
				physicsBody->setDynamic(false);
				physicsBody->setCategoryBitmask(EXIT_M);
				physicsBody->setCollisionBitmask(HERO_M);
				physicsBody->setContactTestBitmask(0xFFFFFFFF);

				sprite->setPhysicsBody(physicsBody);
				frontGroundLayer->addChild(sprite, 0);
			}
		}

	}
	arrObj = tileMap->getObjectGroup("bounds")->getObjects();
	for (auto object : arrObj)
	{
		auto dic = object.asValueMap();
		float width = dic.at("width").asFloat();
		float height = dic.at("height").asFloat();
		float x = dic.at("x").asFloat();
		float y = dic.at("y").asFloat();// +height;

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
		frontGroundLayer->addChild(sprite, 0);
	}
}

bool GameScene::init()
{
	if (!Scene::initWithPhysics())
	{
		return false;
	}
	
	initMap("map/chapter0.tmx",Color4B::Color4B(39,185,154,255));
	initBackgroundMusic();
	initDashboard();
	initListener();

	/*followEnemy = FollowEnemy::create(hero);
	followEnemy->retain();
	frontGroundLayer->addChild(followEnemy, 1);*/

	revivePoint = hero->getPosition();
	heroDied = heroJumped = 0;
	gotGameKey = 0; needGameKey = 0;

	this->schedule(schedule_selector(GameScene::heroUpdate));
	this->schedule(schedule_selector(GameScene::mapUpdate));
	this->schedule(schedule_selector(GameScene::regenerateUpdate));
	this->schedule(schedule_selector(GameScene::messageUpdate));

	return true;
}
void GameScene::initMap(const std::string & tmxFile, const Color4B &backgroundColor){
	auto tileMap = TMXTiledMap::create(tmxFile);

	mapSize = Size(tileMap->getMapSize().width*tileMap->getTileSize().width,
		tileMap->getMapSize().height*tileMap->getTileSize().height);

	frontGroundLayer = Layer::create();
	frontGroundLayer->setAnchorPoint(Vec2::ZERO);
	frontGroundLayer->setPosition(Vec2::ZERO);
	frontGroundLayer->setContentSize(mapSize);
	this->addChild(frontGroundLayer, 0);

	backGroundLayer = Layer::create();
	backGroundLayer->setAnchorPoint(Vec2::ZERO);
	backGroundLayer->setPosition(Vec2::ZERO);
	backGroundLayer->setContentSize(mapSize);
	this->addChild(backGroundLayer, -1);

	//physics
	auto physicsWorld = this->getPhysicsWorld();
	physicsWorld->setGravity(Vec2(0, -1500));
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

	//background
	auto layerColor = LayerColor::create(backgroundColor);
	layerColor->setContentSize(mapSize);
	backGroundLayer->addChild(layerColor, -1);
	auto objGroups = tileMap->getObjectGroups();
	for (auto objGroup : objGroups) {
		if (objGroup->getGroupName() != "game")
			drawBackGround(objGroup->getObjects());
	}
	
	//map
	drawMap(tileMap);
}
/*void GameScene::initMap() {
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
		if (i == howMany - 4) {
			auto door = Door::create("parkour_images/newgameB_rotate.png");
			door->setAnchorPoint(Vec2(0, 0));
			door->setTrack(sX, sY+testSprite->getContentSize().height,sY + testSprite->getContentSize().height +100);
			door->setTag(DOOR_T);
			auto doorKey = DoorKey::create("parkour_images/accelerate.png");
			doorKey->setAnchorPoint(Vec2::ZERO);
			doorKey->setPosition(door->getPositionX()-100,door->getPositionY());
			doorKey->setDoor(door);
			doorKey->setTag(DOOR_KEY_T);

			frontGroundLayer->addChild(doorKey, 1);
			frontGroundLayer->addChild(door, 1);
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
}*/
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

void GameScene::initDashboard(){
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	int padding = 50;

	auto coinScoreBoard = Node::create();
	std::stringstream sstr;  sstr << coinCount << " / " << coinTotal;
	auto scoreLabel = Label::createWithTTF(sstr.str(), "fonts/GermaniaOne-Regular.ttf", 64);
	scoreLabel->setAnchorPoint(Vec2(1.0f, 1.0f)); scoreLabel->setPosition(Vec2::ZERO);
	scoreLabel->setName("ScoreLabel");

	auto label = Label::createWithTTF("Coin", "fonts/GermaniaOne-Regular.ttf", 64);
	label->setAnchorPoint(Vec2(1.0f, 1.0f));
	label->setPosition(Vec2(-(scoreLabel->getContentSize().width) - padding, 0.0f));

	coinScoreBoard->addChild(label, 0);
	coinScoreBoard->addChild(scoreLabel, 0);
	coinScoreBoard->setName("CoinScoreBoard");
	coinScoreBoard->setAnchorPoint(Vec2(1.0f, 1.0f));
	coinScoreBoard->setPosition(Vec2(visibleSize.width - padding, visibleSize.height - padding));

	this->addChild(coinScoreBoard, 100);
}

void GameScene::initBackgroundMusic(){
	if (AUDIO_PLAY)
		SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/spring_music.wav", true);
}

bool GameScene::onContactBegin(cocos2d::PhysicsContact &contact) {
	//log("contact begin");
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	const Vec2 contactPoint=contact.getContactData()->points[0];

	if (nodeA->getTag() == DOOR_KEY_T) {
		auto doorKey = (DoorKey*)nodeA;
		doorKey->lock();
	}
	if (nodeB->getTag() == DOOR_KEY_T) {
		auto doorKey = (DoorKey*)nodeB;
		doorKey->lock();
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

	if (isHero(nodeA) && nodeB->getTag() == FOLLOW_ENEMY_T) {
		heroDied = 1;
		//log("die contact");
		return false;
	}

	if (isHero(nodeA)) {
		auto hero = (Hero*)nodeA;
		if (nodeB->getTag() == ENEMY_T) {
			/*if (touchUpSurface(hero, nodeB)) {
				nodeB->retain();
				destroyedList.push_back(nodeB);
				nodeB->removeFromParent();
				return false;
			}
			else {*/
			heroDied = 1;
			return false;
			//}
		}
		if ((nodeB->getTag() == BORDER_T && contactPoint.y <= 10.0f)||
			(nodeB->getTag()==TRAP_T /*&& touchUpSurface(hero,nodeB)*/) ||
			(nodeB->getTag() == SLIDING_TRAP_T /*&& touchUpSurface(hero, nodeB)*/ )){
			//log("die");
			heroDied = 1;
			return false;
		}
		if (nodeB->getTag() == COIN_T) {
			coinCount++;
			nodeB->retain();
			destroyedList.push_back(nodeB);
			nodeB->removeFromParent();
			return false;
		}
		if (nodeB->getTag() == GAME_KEY_T) {
			gotGameKey = 1;
			nodeB->retain();
			destroyedList.push_back(nodeB);
			nodeB->removeFromParent();
			return false;
		}
		if (nodeB->getTag() == EXIT_T) {
			if (gotGameKey || !needGameKey)
				gamePass();
			else
				messageSingleLine("No key to open it...");
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
					hero->addOnGround();
					*heroSetOnGround = true;
					//log("on ground");
					if (nodeB->getTag() == SLIDING_LAND_T) {
						hero->setSlidingGround((Sprite*)nodeB);
					}
					if (nodeB->getTag() == DROP_LAND_T) {
						auto dropLand = (DropLand*)nodeB;
						dropLand->drop();
					}
					if (nodeB->getName() == "RevivePoint") {
						setRevivePoint(Vec2(nodeB->getPositionX()+nodeB->getContentSize().width/2-hero->getContentSize().width/2,
							nodeB->getPositionY()+nodeB->getContentSize().height+20.0f));
						//nodeB->setName("");
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
			auto hero = (Hero*)nodeA;
			hero->decOnGround();
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
	if (keyCode == EventKeyboard::KeyCode::KEY_1) {
		followEnemy->startFollow();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_2) {
		followEnemy->stopFollow();
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
	if (!hero->getOnGround()) {
		if (rightKeyDown)
			hero->rightJump();
		else if (leftKeyDown)
			hero->leftJump();
		else
			hero->jump();
	}
	else {
		if (rightKeyDown)
			hero->right();
		else if (leftKeyDown)
			hero->left();
		else {
			if (lastKey == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) { hero->rightSilence(); }
			else if (lastKey == EventKeyboard::KeyCode::KEY_LEFT_ARROW) { hero->leftSilence(); }
			else hero->rightSilence();
		}
	}

	auto delta = 400.0f*(1.0f-hero->getPhysicsBody()->getLinearDamping());
	auto velocity = hero->getPhysicsBody()->getVelocity();
	
	//velocityX
	velocity.x = 0;
	if (rightKeyDown) velocity.x += delta;
	if (leftKeyDown) velocity.x -= delta;
	if (hero->getSlidingGround() != nullptr) {
		auto slidingVel = hero->getSlidingGround()->getPhysicsBody()->getVelocity();
		velocity.x += slidingVel.x;
	}

	//velocityY
	if (hero->getSlidingGround() != nullptr) {
		auto slidingVel = hero->getSlidingGround()->getPhysicsBody()->getVelocity();
		velocity.y = slidingVel.y;
	}
	if (heroJumped) {
		velocity.y = 650.0f*(1.0f - hero->getPhysicsBody()->getLinearDamping());
		heroJumped = 0;
	}
	velocity.y = std::max(-800.0f, velocity.y);

	hero->getPhysicsBody()->setVelocity(velocity);
}
void GameScene::mapUpdate(float dt) {
	//camera
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
		backGroundLayer->setPosition(layerPosition);
	}
	//dashboard
	std::stringstream sstr;  sstr << coinCount << " / " << coinTotal;
	auto scoreLabel=(Label*)this->getChildByName("CoinScoreBoard")->getChildByName("ScoreLabel");
	scoreLabel->setString(sstr.str());
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

void GameScene::messageUpdate(float dt)
{
}

void GameScene::setRevivePoint(Vec2 revive)
{
	revivePoint = revive;
	destroyedList.clear();
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
	if (followEnemy != nullptr)
		followEnemy->stopFollow();
	Director::getInstance()->pushScene(ReviveScene::createScene());
	hero->resetOnGround();
	hero->setPosition(revivePoint);
	//heroDied = 0; // onEnter
}
void GameScene::heroJump() {
	heroJumped = 1;
	hero->addJumpTimes();
	if (AUDIO_PLAY)
		SimpleAudioEngine::getInstance()->playEffect("sounds/jump.wav", false, 1.0f, 1.0f, 1.0f);
}

void GameScene::messageSingleLine(const std::string & mes)
{
	float padding = 50,sx = std::max(400.0f,mes.length()*25.0f+64+64), sy = 200;
	auto nodeItems = Node::create();

	auto messageBox = Sprite::create("ui/messageBox.png");
	messageBox->setContentSize(Size(sx, sy));

	auto line = Label::createWithTTF(mes, "fonts/GermaniaOne-Regular.ttf", 64);
	line->setAnchorPoint(Vec2(0.0, 0.5));
	line->setPosition(Vec2(-sx/2+64,0.0f));

	nodeItems->addChild(messageBox, 0);
	nodeItems->addChild(line,1);

	auto moveTo = MoveTo::create(0.5f, 
		Vec2(padding + sx / 2, Director::getInstance()->getVisibleSize().height - padding - sy / 2));
	auto fadeOut = FadeOut::create(2.0f);
	auto remove = CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, nodeItems));
	nodeItems->setPosition(padding + sx / 2, 
		Director::getInstance()->getVisibleSize().height + padding + sy / 2);

	nodeItems->runAction(Sequence::create(moveTo,DelayTime::create(4.0f+2.0f), remove, nullptr));
	line->runAction(Sequence::create(DelayTime::create(0.5f + 4.0f), fadeOut->clone(), nullptr));
	messageBox->runAction(Sequence::create(DelayTime::create(0.5f + 4.0f), fadeOut->clone(), nullptr));

	this->addChild(nodeItems, 10);

}

void GameScene::messageDoubleLine(const std::string & mes1, const std::string & mes2)
{
	float padding = 50;
	float sx = std::max(400.0f,std::max(mes1.length() * 20.0f + 64 + 64, mes2.length() * 20.0f + 64 + 64)), sy = 200;
	auto nodeItems = Node::create();

	auto messageBox = Sprite::create("ui/messageBox.png");
	messageBox->setContentSize(Size(sx, sy));

	auto line1 = Label::createWithTTF(mes1, "fonts/GermaniaOne-Regular.ttf", 48);
	line1->setAnchorPoint(Vec2(0.0, 0.5));
	line1->setPosition(Vec2(-sx / 2 + 64, 32));

	auto line2 = Label::createWithTTF(mes2, "fonts/GermaniaOne-Regular.ttf", 48);
	line2->setAnchorPoint(Vec2(0.0, 0.5));
	line2->setPosition(Vec2(-sx / 2 + 64, -32));

	nodeItems->addChild(messageBox, 0);
	nodeItems->addChild(line1, 1);
	nodeItems->addChild(line2, 2);

	auto moveTo = MoveTo::create(0.5f,
		Vec2(padding + sx / 2, Director::getInstance()->getVisibleSize().height - padding - sy / 2));
	auto fadeOut = FadeOut::create(2.0f);
	auto remove = CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, nodeItems));
	nodeItems->setPosition(padding + sx / 2,
		Director::getInstance()->getVisibleSize().height + padding + sy / 2);

	nodeItems->runAction(Sequence::create(moveTo, DelayTime::create(4.0f + 2.0f), remove, nullptr));
	line1->runAction(Sequence::create(DelayTime::create(0.5f + 4.0f), fadeOut->clone(), nullptr));
	line2->runAction(Sequence::create(DelayTime::create(0.5f + 4.0f), fadeOut->clone(), nullptr));
	messageBox->runAction(Sequence::create(DelayTime::create(0.5f + 4.0f), fadeOut->clone(), nullptr));

	this->addChild(nodeItems, 10);
}

void GameScene::switchScene(float dt)
{
	Director::getInstance()->replaceScene(HelloScene::createScene());
}

void GameScene::win()
{
	this->unscheduleAllCallbacks();
	_eventDispatcher->removeEventListenersForTarget(this);

	auto exit=(Sprite*)frontGroundLayer->getChildByName("Exit");
	exit->setTexture("map/exit.png");

	auto heroModel = Sprite::createWithTexture(hero->getTexture());
	auto jumpBy = JumpBy::create(0.8f, Vec2::ZERO, 100, 2);
	auto delay = DelayTime::create(1.0f);

	heroModel->setAnchorPoint(Vec2::ZERO);
	heroModel->setPosition(hero->getPosition());
	heroModel->runAction(RepeatForever::create(Sequence::create(jumpBy, delay, nullptr)));
	frontGroundLayer->addChild(heroModel, 10);

	auto firework = Sprite::create("hero/firework.png");
	firework->setPosition(heroModel->getPosition() + heroModel->getContentSize()*0.5); 
	firework->runAction(RepeatForever::create(Sequence::create(jumpBy->clone(), delay->clone(), nullptr)));
	frontGroundLayer->addChild(firework, 9);

	//hero->setVisible(false);
	hero->removeFromParent();
	
	scheduleOnce(schedule_selector(GameScene::switchScene), 4.0f);
}

void GameScene::gamePass()
{
	win();
}
