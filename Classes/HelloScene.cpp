#include "HelloScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloScene::createScene()
{
	//Size visibleSize = Director::getInstance()->getVisibleSize();
	//return HelloScene::createWithSize(Size(visibleSize.width,visibleSize.height));
	return HelloScene::create();
}

// on "init" you need to initialize your instance
bool HelloScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto layer = Layer::create();
	//layer->setContentSize(Size(visibleSize.width*2, visibleSize.height*2));
	//layer->runAction(MoveBy::create(5,Vec2(-visibleSize.width, -visibleSize.height)));
	Size layerSize = layer->getContentSize();

	auto parent = layer;

	this->addChild(layer,0);

	auto backGround = Sprite::create("parkour_images/back_spring.png");
	backGround->setContentSize(layerSize);
	backGround->setAnchorPoint(Vec2::ZERO);
	backGround->setPosition(Vec2::ZERO);
	parent->addChild(backGround, -1);
	
	Size playingSize = Size(layerSize.width, layerSize.height - (layerSize.height / 8)); // actual playing size to work with

	auto color = Color4F(1.0f, 0.5f, 0.3f, 1);

	int verts = 4;

	Vec2 stripe1[] = {
		Vec2::ZERO,
		Vec2(0,200),
		Vec2(600,200),
		Vec2(600,0)
	};

	DrawNode* dotNode1 = DrawNode::create();
	dotNode1->setContentSize(Size(600, 200));
	dotNode1->drawPolygon(stripe1, verts, color, 0, color);

	dotNode1->setPosition(Vec2(layerSize.width / 2 - (dotNode1->getContentSize().width / 2),
		playingSize.height - dotNode1->getContentSize().height));

	auto label = Label::createWithTTF("Super Cocos Aliens", "fonts/Marker Felt.ttf", 64);
	dotNode1->addChild(label, 1);
	auto dotNodeSize = dotNode1->getContentSize();
	label->setPosition(Vec2(dotNodeSize.width / 2, dotNodeSize.height / 2));

	parent->addChild(dotNode1, -1);

	int paddingX = 20;
	int paddingY = 20;

	auto labelNode = Node::create();

	auto player1 = Label::createWithTTF("Player 1", "fonts/Marker Felt.ttf", 32);
	auto player2 = Label::createWithTTF("Player 2", "fonts/Marker Felt.ttf", 32);

	auto player1Score = Label::createWithTTF("00000", "fonts/Marker Felt.ttf", 32);
	auto player2Score = Label::createWithTTF("00000", "fonts/Marker Felt.ttf", 32);

	labelNode->addChild(player1, 0);
	labelNode->addChild(player2, 0);
	labelNode->addChild(player1Score, 0);
	labelNode->addChild(player2Score, 0);

	auto player1Size = player1->getContentSize();
	auto player1ScoreSize = player1Score->getContentSize();
	auto player2Size = player2->getContentSize();
	auto player2ScoreSize = player2Score->getContentSize();

	player1->setPosition(Vec2(0 + player1Size.width / 2 + paddingX,
		layerSize.height - player1Size.height / 2 - paddingY));

	player1Score->setPosition(Vec2(0 + player1->getPositionX() + player1ScoreSize.width + paddingX,
		layerSize.height - player1ScoreSize.height / 2 - paddingY));

	player2Score->setPosition(Vec2(layerSize.width - player2ScoreSize.width / 2 - paddingX,
		layerSize.height - player2ScoreSize.height / 2 - paddingY));

	player2->setPosition(Vec2(player2Score->getPositionX() - player2Size.width - paddingX,
		layerSize.height - player2Size.height / 2 - paddingY));

	parent->addChild(labelNode, -1);


	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// create a node to hold non-sprites.
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	auto nodeItems = Node::create();

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// create a path/walkway
	// depending upon how large the screen is we need to decide how many blocks to lay down.
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	auto testSprite = Sprite::create("chapter2/ZigzagForest_Square.png");
	auto spriteSize = testSprite->getContentSize();

	testSprite->setScale(Director::getInstance()->getContentScaleFactor());

	int howMany = std::ceil(layerSize.width / spriteSize.width);

	int sX = 0; // act as a counter for setPosition x coordinate.
	int sY = 0; // act as a counter for setPosition y coordinate.

	playingSize = Size(layerSize.width, layerSize.height - spriteSize.height);

	for (int i = 0; i < howMany; i++)
	{
		auto sprite = Sprite::create("chapter2/ZigzagForest_Square.png");
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setPosition(sX, sY);

		sX += sprite->getContentSize().width;

		nodeItems->addChild(sprite, 0);
	}

	testSprite = NULL;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// create the blocks
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	testSprite = Sprite::create("chapter2/ZigzagGrass_Mud_Round.png");

	sX = layerSize.width / 2 - testSprite->getContentSize().width;
	sY = playingSize.height / 2 - testSprite->getContentSize().height * 2;

	for (int i = 0; i < 5; i++)
	{
		auto sprite = Sprite::create("chapter2/ZigzagGrass_Mud_Round.png");
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setPosition(sX, sY);

		sX += sprite->getContentSize().width;

		nodeItems->addChild(sprite, 0);
	}

	testSprite = NULL;

	parent->addChild(nodeItems, 1);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// create a node to hold all sprites
	// create  a few sprites
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	auto spriteNode = Node::create();

	auto sprite1 = Sprite::create("chapter2/Blue_Front1.png");
	sprite1->setAnchorPoint(Vec2(0, 0));
	sprite1->setPosition(layerSize.width*1/5, (layerSize.height - playingSize.height));
	spriteNode->addChild(sprite1, 1);

	auto sprite2 = Sprite::create("chapter2/LightBlue_Front1.png");
	sprite2->setAnchorPoint(Vec2(0, 0));
	sprite2->setPosition(layerSize.width * 3 / 5, (layerSize.height - playingSize.height));
	spriteNode->addChild(sprite2, 1);

	auto sprite3 = Sprite::create("chapter2/White_Front1.png");
	sprite3->setAnchorPoint(Vec2(0, 0));
	sprite3->setPosition(layerSize.width * 4 / 5, (layerSize.height - playingSize.height));
	spriteNode->addChild(sprite3, 1);

	parent->addChild(spriteNode, 1);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// create a node to hold menu
	// create a menu
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	auto menuItem1 = MenuItemFont::create("Start Game");
	menuItem1->setColor(Color3B::ORANGE);
	menuItem1->setFontNameObj("fonts/Marker Felt.ttf");
	menuItem1->setFontSizeObj(64);

	menuItem1->setCallback([&](Ref *sender) {
		Director::getInstance()->replaceScene(GameScene::createScene());
	});

	auto menu = Menu::create(menuItem1, NULL);
	menu->setPosition(layerSize.width / 2, layerSize.height / 2);
	parent->addChild(menu, 2);

	//parent->setPosition(-100, -100);

	return true;
}


void HelloScene::menuExitCallback(Ref* pSender)
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
