#include "HelloScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Chapter0Level1.h"
#include "Chapter0Level2.h"
#include "OptionScene.h"
#include "EntryScene.h"
#include "HelpScene.h"
using namespace CocosDenshion;
USING_NS_CC;

Scene* HelloScene::createScene()
{
	return HelloScene::create();
}

void HelloScene::onEnterTransitionDidFinish() {
	for (auto menuItem:menuItems) 
		menuItem->unselected();
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
	title->setContentSize(title->getContentSize()*2.0f);
	title->setPosition(visibleSize.width / 2, visibleSize.height / 4 * 3);
	this->addChild(title, 1);

	auto heroModel = Sprite::create("map/hero.png");
	heroModel->setAnchorPoint(Vec2::ZERO);
	heroModel->setPosition(Vec2(330,703));
	auto jumpBy = JumpBy::create(0.8f, Vec2::ZERO, 100, 2);
	auto delay = DelayTime::create(1.0f);
	heroModel->runAction(RepeatForever::create(Sequence::create(jumpBy, delay, nullptr)));

	this->addChild(heroModel, 1);

	int height = 128,padding=0,sy=0;

	auto menuItemStart = MenuItemImage::create("ui/buttonStartNormal.png", "ui/buttonStartSelected.png");
	menuItemStart->setCallback([&](Ref *sender) {
		Director::getInstance()->replaceScene(EntryScene::createScene());
	});
	menuItemStart->setPositionY(sy+(height+padding)*3);
	menuItemStart->retain(); menuItems.push_back(menuItemStart);

	auto menuItemHelp = MenuItemImage::create("ui/buttonHelpNormal.png", "ui/buttonHelpSelected.png");
	menuItemHelp->setCallback([&](Ref *sender) {
		Director::getInstance()->pushScene(HelpScene::createScene());
	});
	menuItemHelp->setPositionY(sy + (height + padding) *2);
	menuItemHelp->retain(); menuItems.push_back(menuItemHelp);

	auto menuItemOption = MenuItemImage::create("ui/buttonOptionNormal.png", "ui/buttonOptionSelected.png");
	menuItemOption->setCallback([&](Ref *sender) {
		Director::getInstance()->pushScene(OptionScene::createScene(true));
	});
	menuItemOption->setPositionY(sy + (height + padding));
	menuItemOption->retain(); menuItems.push_back(menuItemOption);

	auto menuItemExit = MenuItemImage::create("ui/buttonExitNormal.png", "ui/buttonExitSelected.png");
	menuItemExit->setCallback([&](Ref *sender) {
		Director::getInstance()->end();
	});
	menuItemExit->setPositionY(sy);
	menuItemExit->retain(); menuItems.push_back(menuItemExit);

	menu = Menu::create(menuItemStart, menuItemHelp, menuItemOption, menuItemExit,NULL);
	menu->setAnchorPoint(Vec2(0.5f, 0.0f));
	menu->setPosition(visibleSize.width / 2, visibleSize.height / 4);
	this->addChild(menu, 2);

	auto arrow = Sprite::create("ui/arrow.png");
	arrow->setName("Arrow");
	this->addChild(arrow, 2);
	arrow->setContentSize(arrow->getContentSize()*1.5f);
	setArrowPosition(curSelect = 0);

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(HelloScene::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(HelloScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	BGM(HELLOBGM);

	return true;
}


bool HelloScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
		menuItems[curSelect]->selected();
	}
	return true;
}
bool HelloScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
		menuItems[curSelect]->activate();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
		setArrowPosition((curSelect = curSelect + menuItems.size() - 1) %= menuItems.size());
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
		setArrowPosition((curSelect = curSelect + 1) %= menuItems.size());
	}
	return true;
}

void HelloScene::setArrowPosition(int pos)
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

