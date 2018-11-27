#include "PauseScene.h"
#include "HelpScene.h"
#include "OptionScene.h"
#include "SimpleAudioEngine.h"
#include "EntryScene.h"

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

	int height = 128, padding = 0, sy = 0;

	auto menuItemResume = MenuItemImage::create("ui/buttonResumeNormal.png", "ui/buttonResumeSelected.png");
	menuItemResume->setCallback([&](Ref *sender) {
		Director::getInstance()->popScene();
	});
	menuItemResume->setPositionY(sy + (height + padding) * 3);

	auto menuItemHelp = MenuItemImage::create("ui/buttonHelpNormal.png", "ui/buttonHelpSelected.png");
	menuItemHelp->setCallback([&](Ref *sender) {
		Director::getInstance()->pushScene(HelpScene::createScene());
	});
	menuItemHelp->setPositionY(sy + (height + padding) * 2);

	auto menuItemOption = MenuItemImage::create("ui/buttonOptionNormal.png", "ui/buttonOptionSelected.png");
	menuItemOption->setCallback([&](Ref *sender) {
		Director::getInstance()->pushScene(OptionScene::createScene());
	});
	menuItemOption->setPositionY(sy + (height + padding));

	auto menuItemExit = MenuItemImage::create("ui/buttonExitNormal.png", "ui/buttonExitSelected.png");
	menuItemExit->setCallback([&](Ref *sender) {
		Director::getInstance()->popToRootScene();
		Director::getInstance()->replaceScene(EntryScene::createScene());
	});
	menuItemExit->setPositionY(sy);

	auto menu = Menu::create(menuItemResume, menuItemHelp, menuItemOption, menuItemExit, NULL);
	menu->setAnchorPoint(Vec2(0.5f, 0.0f));
	menu->setPosition(visibleSize.width / 2, visibleSize.height / 4);
	this->addChild(menu, 2);

	return true;
}