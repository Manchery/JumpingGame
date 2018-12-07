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

void PauseScene::onEnterTransitionDidFinish() {
	for (int i = 0; i < menuItems.size(); i++) {
		menuItems[i]->unselected();
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
		RESUMEBGM;
		Director::getInstance()->popScene();
	});
	menuItemResume->setPositionY(sy + (height + padding) * 3);
	menuItemResume->retain(); menuItems.push_back(menuItemResume);

	auto menuItemRestart = MenuItemImage::create("ui/buttonRestartNormal.png", "ui/buttonRestartSelected.png");
	menuItemRestart->setCallback([&](Ref *sender) {
		restart();
	});
	menuItemRestart->setPositionY(sy + (height + padding) * 2);
	menuItemRestart->retain(); menuItems.push_back(menuItemRestart);

	auto menuItemHelp = MenuItemImage::create("ui/buttonHelpNormal.png", "ui/buttonHelpSelected.png");
	menuItemHelp->setCallback([&](Ref *sender) {
		Director::getInstance()->pushScene(HelpScene::createScene());
	});
	menuItemHelp->setPositionY(sy + (height + padding));
	menuItemHelp->retain(); menuItems.push_back(menuItemHelp);

	auto menuItemExit = MenuItemImage::create("ui/buttonExitNormal.png", "ui/buttonExitSelected.png");
	menuItemExit->setCallback([&](Ref *sender) {
		Director::getInstance()->popToRootScene();
		Director::getInstance()->replaceScene(EntryScene::createScene());
	});
	menuItemExit->setPositionY(sy);
	menuItemExit->retain(); menuItems.push_back(menuItemExit);

	menu = Menu::create(menuItemResume, menuItemRestart,menuItemHelp, menuItemExit, NULL);
	menu->setAnchorPoint(Vec2(0.5f, 0.0f));
	menu->setPosition(visibleSize.width / 2, visibleSize.height / 4);
	((HelloScene*)this)->addChild(menu, 2);
	
	auto arrow = Sprite::create("ui/arrow.png");
	arrow->setName("Arrow");
	this->addChild(arrow, 2);
	arrow->setContentSize(arrow->getContentSize()*1.5f);
	setArrowPosition(curSelect = 0);

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(PauseScene::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(PauseScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	PAUSEBGM;
	return true;
}

bool PauseScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
		menuItems[curSelect]->selected();
	}
	return true;
}
bool PauseScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
		menuItems[curSelect]->activate();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
		setArrowPosition((curSelect = curSelect + menuItems.size()-1) %= menuItems.size());
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
		setArrowPosition((curSelect = curSelect + 1) %= menuItems.size());
	}
	return true;
}

void PauseScene::setArrowPosition(int pos)
{
	auto arrow = (Sprite*)this->getChildByName("Arrow");
	auto seq = Sequence::create(DelayTime::create(0.25f), ScaleTo::create(0.5f, 1, 0.5), ScaleTo::create(0.25f, 1, 0),
		ScaleTo::create(0.25f, 1, 0.5), ScaleTo::create(0.5f, 1, 1), DelayTime::create(0.25f), nullptr);
	arrow->stopAllActions();
	arrow->setScale(1.0f);
	arrow->setPosition(menuItems[pos]->getPosition() + menu->getPosition() -
		Vec2(menuItems[pos]->getContentSize().width / 2 + 50, -5));
	arrow->runAction(RepeatForever::create(seq));
}

void PauseScene::restart()
{
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
}
