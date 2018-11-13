#include "HelpScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelpScene::createScene()
{
	return HelpScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelpScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Size playingSize = Size(visibleSize.width, visibleSize.height - (visibleSize.height / 8)); // actual playing size to work with

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

	dotNode1->setPosition(Vec2(visibleSize.width / 2 - (dotNode1->getContentSize().width / 2),
		playingSize.height - dotNode1->getContentSize().height));

	auto label = Label::createWithTTF("Super Cocos Aliens", "fonts/Marker Felt.ttf", 64);
	dotNode1->addChild(label, 1);
	auto dotNodeSize = dotNode1->getContentSize();
	label->setPosition(Vec2(dotNodeSize.width / 2, dotNodeSize.height / 2));

	this->addChild(dotNode1, -1);

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
		visibleSize.height - player1Size.height / 2 - paddingY));

	player1Score->setPosition(Vec2(0 + player1->getPositionX() + player1ScoreSize.width + paddingX,
		visibleSize.height - player1ScoreSize.height / 2 - paddingY));

	player2Score->setPosition(Vec2(visibleSize.width - player2ScoreSize.width / 2 - paddingX,
		visibleSize.height - player2ScoreSize.height / 2 - paddingY));

	player2->setPosition(Vec2(player2Score->getPositionX() - player2Size.width - paddingX,
		visibleSize.height - player2Size.height / 2 - paddingY));

	this->addChild(labelNode, -1);


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

	int howMany = std::ceil(visibleSize.width / spriteSize.width);

	int sX = 0; // act as a counter for setPosition x coordinate.
	int sY = 0; // act as a counter for setPosition y coordinate.

	playingSize = Size(visibleSize.width, visibleSize.height - spriteSize.height);

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

	sX = visibleSize.width / 2 - testSprite->getContentSize().width;
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

	this->addChild(nodeItems, 1);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// create a node to hold all sprites
	// create  a few sprites
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	auto spriteNode = Node::create();

	auto sprite1 = Sprite::create("chapter2/Blue_Front1.png");
	sprite1->setAnchorPoint(Vec2(0, 0));
	sprite1->setPosition(100, (visibleSize.height - playingSize.height));
	spriteNode->addChild(sprite1, 1);

	auto sprite2 = Sprite::create("chapter2/LightBlue_Front1.png");
	sprite2->setAnchorPoint(Vec2(0, 0));
	sprite2->setPosition(500, (visibleSize.height - playingSize.height));
	spriteNode->addChild(sprite2, 1);

	auto sprite3 = Sprite::create("chapter2/White_Front1.png");
	sprite3->setAnchorPoint(Vec2(0, 0));
	sprite3->setPosition(800, (visibleSize.height - playingSize.height));
	spriteNode->addChild(sprite3, 1);

	this->addChild(spriteNode, 1);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// create a node to hold menu
	// create a menu
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	auto menuNode = Node::create();

	auto menuItem1 = MenuItemFont::create("Start Game");
	menuItem1->setFontNameObj("fonts/Marker Felt.ttf");
	menuItem1->setFontSizeObj(64);

	menuItem1->setCallback([&](Ref *sender) {
		Director::getInstance()->replaceScene(HelloWorld::createScene());
	});

	auto menu = Menu::create(menuItem1, NULL);
	menuNode->addChild(menu, 0);

	this->addChild(menuNode, 2);

	return true;
}


void HelpScene::menuExitCallback(Ref* pSender)
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
