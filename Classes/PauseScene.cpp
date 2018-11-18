#include "PauseScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* PauseScene::createScene()
{
	return PauseScene::create();
}
bool PauseScene::init()
{
	if (!TempScene::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto menuItem1 = MenuItemFont::create("Resume");
	menuItem1->setColor(Color3B::ORANGE);
	menuItem1->setFontNameObj("fonts/Marker Felt.ttf");
	menuItem1->setFontSizeObj(64);

	menuItem1->setCallback([&](Ref *sender) {
		Director::getInstance()->popScene();
	});

	auto menu = Menu::create(menuItem1, NULL);
	menu->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this ->addChild(menu, 2);

	return true;
}