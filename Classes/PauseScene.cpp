#include "PauseScene.h"
#include "HelpScene.h"
#include "OptionScene.h"
#include "SimpleAudioEngine.h"
#include "Chapter0Level1.h"
#include "Chapter1Level1.h"
#include "Chapter2Level1.h"
#include "Chapter3Level1.h"
#include "Chapter3Level2.h"
#include "Chapter4Level1.h"
#include "Chapter5Level1.h"
#include "Chapter6Level1.h"
#include "EntryScene.h"

Scene* PauseScene::createScene(int chapterId)
{
	PauseScene *scene = new (std::nothrow) PauseScene();
	if (scene && scene->init(chapterId))
	{
		scene->autorelease();
		return scene;
	}
	else
	{
		CC_SAFE_DELETE(scene);
		return nullptr;
	}
}
bool PauseScene::init(int chapterId)
{
	if (!TempScene::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	chapterID=chapterId;

	int height = 128, padding = 0, sy = 0;

	auto menuItemResume = MenuItemImage::create("ui/buttonResumeNormal.png", "ui/buttonResumeSelected.png");
	menuItemResume->setCallback([&](Ref *sender) {
		Director::getInstance()->popScene();
	});
	menuItemResume->setPositionY(sy + (height + padding) * 3);

	auto menuItemRestart = MenuItemImage::create("ui/buttonRestartNormal.png", "ui/buttonRestartSelected.png");
	menuItemRestart->setCallback([&](Ref *sender) {
		Director::getInstance()->popToRootScene();
		Scene* scene; int idx = chapterID;
		if (idx == -1)
			scene = Chapter0Level1::createScene();
		else if (idx == 0)
			scene = Chapter0Level1::createScene();
		else if (idx == 1)
			scene = Chapter1Level1::createScene();
		else if (idx == 2)
			scene = Chapter2Level1::createScene();
		else if (idx == 3)
			scene = Chapter3Level1::createScene();
		else if (idx == 4)
			scene = Chapter4Level1::createScene();
		else if (idx == 5)
			scene = Chapter5Level1::createScene();
		else if (idx == 6)
			scene = Chapter6Level1::createScene();
		Director::getInstance()->replaceScene(TransitionCrossFade::create(2.0f, scene));
	});
	menuItemRestart->setPositionY(sy + (height + padding) * 2);

	auto menuItemHelp = MenuItemImage::create("ui/buttonHelpNormal.png", "ui/buttonHelpSelected.png");
	menuItemHelp->setCallback([&](Ref *sender) {
		Director::getInstance()->pushScene(HelpScene::createScene());
	});
	menuItemHelp->setPositionY(sy + (height + padding));

	auto menuItemExit = MenuItemImage::create("ui/buttonExitNormal.png", "ui/buttonExitSelected.png");
	menuItemExit->setCallback([&](Ref *sender) {
		Director::getInstance()->popToRootScene();
		Director::getInstance()->replaceScene(EntryScene::createScene());
	});
	menuItemExit->setPositionY(sy);

	auto menu = Menu::create(menuItemResume, menuItemRestart,menuItemHelp, menuItemExit, NULL);
	menu->setAnchorPoint(Vec2(0.5f, 0.0f));
	menu->setPosition(visibleSize.width / 2, visibleSize.height / 4);
	this->addChild(menu, 2);

	return true;
}