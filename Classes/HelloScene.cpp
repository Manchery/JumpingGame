#include "HelloScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Chapter0Level1.h"

USING_NS_CC;

Scene* HelloScene::createScene()
{
	return HelloScene::create();
}

bool HelloScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backGround = Sprite::create("ui/helloground.png");
	backGround->setContentSize(visibleSize);
	backGround->setAnchorPoint(Vec2::ZERO);
	backGround->setPosition(Vec2::ZERO);
	this->addChild(backGround, -1);

	auto title = Sprite::create("ui/title.png");
	title->setPosition(visibleSize.width / 2, visibleSize.height / 4 * 3);
	this->addChild(title, 1);

	auto heroModel = Sprite::create("map/hero.png");
	heroModel->setPosition(100, 100);
	auto jumpBy = JumpBy::create(0.8f, Vec2::ZERO, 100, 2);
	auto delay = DelayTime::create(1.0f);
	heroModel->runAction(RepeatForever::create(Sequence::create(jumpBy, delay, nullptr)));

	this->addChild(heroModel, 1);

	auto menuItemStart = MenuItemImage::create("ui/buttonStartNormal.png", "ui/buttonStartSelected.png");
	menuItemStart->setCallback([&](Ref *sender) {
		Director::getInstance()->replaceScene(Chapter0Level1::createScene());
	});
	menuItemStart->setPositionY(180);

	auto menuItemHelp = MenuItemImage::create("ui/buttonHelpNormal.png", "ui/buttonHelpSelected.png");
	menuItemHelp->setCallback([&](Ref *sender) {
		Director::getInstance()->replaceScene(GameScene::createScene());
	});
	menuItemHelp->setPositionY(120);

	auto menuItemOption = MenuItemImage::create("ui/buttonOptionNormal.png", "ui/buttonOptionSelected.png");
	menuItemOption->setCallback([&](Ref *sender) {
		Director::getInstance()->replaceScene(GameScene::createScene());
	});
	menuItemOption->setPositionY(60);

	auto menuItemExit = MenuItemImage::create("ui/buttonExitNormal.png", "ui/buttonExitSelected.png");
	menuItemExit->setCallback([&](Ref *sender) {
		Director::getInstance()->end();
	});
	menuItemExit->setPositionY(0);

	auto menu = Menu::create(menuItemStart, menuItemHelp, menuItemOption, menuItemExit,NULL);
	menu->setAnchorPoint(Vec2(0.5f, 0.0f));
	menu->setPosition(visibleSize.width / 2, visibleSize.height / 4);
	this->addChild(menu, 2);

	return true;
}
