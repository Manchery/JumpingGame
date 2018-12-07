#include "HelpScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include <sstream>

USING_NS_CC;
using namespace cocos2d::ui;

Scene* HelpScene::createScene()
{
	return HelpScene::create();
}

bool HelpScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//background

	auto backGround = Sprite::create("ui/helloground.png");
	backGround->setContentSize(visibleSize);
	backGround->setAnchorPoint(Vec2::ZERO);
	backGround->setPosition(Vec2::ZERO);
	this->addChild(backGround, -1);

	//back to main scene

	auto backButton = Button::create("ui/buttonBackNormal.png", "ui/buttonBackSelected.png");
	backButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type) {
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED: {
			Director::getInstance()->popScene();
		}break;
		default:
			break;
		}
	});
	backButton->setAnchorPoint(Vec2(0.5,0));
	backButton->setPosition(Vec2(visibleSize.width/2,80));
	this->addChild(backButton);

	// left-right button

	auto leftButton = Button::create("ui/leftTriangle.png");
	leftButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type) {
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED: {
			previousPage();
		}break;
		default:
			break;
		}
	});
	leftButton->setName("LeftButton");
	leftButton->setPosition(Vec2(-visibleSize.width / 4 + visibleSize.width / 2  -10 + 15, visibleSize.height / 2 + 50));
	this->addChild(leftButton, 2);

	leftButton->setVisible(false);

	auto rightButton = Button::create("ui/rightTriangle.png");
	rightButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type) {
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED: {
			nextPage();
		}break;
		default:
			break;
		}
	});
	rightButton->setName("RightButton");
	rightButton->setPosition(Vec2(visibleSize.width / 4 + visibleSize.width / 2  +10 + 15, visibleSize.height / 2 + 50));
	this->addChild(rightButton, 2);

	//page

	helpPageTotal = 5;
	for (int i = 1; i <= 5; i++) {
		helpPages.push_back(Sprite::create("ui/helppage" + std::to_string(i) + ".png")->getTexture());
	}

	auto helpPage = Sprite::create("ui/helppage1.png");
	helpPage->setName("HelpPage");
	helpPage->setPosition(Vec2(visibleSize/2)+Vec2(0,50));
	iter = 0;
	this->addChild(helpPage);

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(HelpScene::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(HelpScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	return true;
}

bool HelpScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	return true;
}
bool HelpScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
		Director::getInstance()->popScene();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
		nextPage();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
		previousPage();
	}
	return true;
}

void HelpScene::nextPage()
{
	if (iter < helpPageTotal - 1) {
		((Sprite*)this->getChildByName("HelpPage"))->setTexture(
			helpPages[(++iter) % helpPageTotal]);

		this->getChildByName("LeftButton")->setVisible(true);
		this->getChildByName("RightButton")->setVisible(true);
		if (iter == 0) this->getChildByName("LeftButton")->setVisible(false);
		if (iter == helpPageTotal - 1) this->getChildByName("RightButton")->setVisible(false);
	}
}

void HelpScene::previousPage()
{
	if (iter > 0) {
		((Sprite*)this->getChildByName("HelpPage"))->setTexture(
			helpPages[(iter += helpPageTotal - 1) %= helpPageTotal]);

		this->getChildByName("LeftButton")->setVisible(true);
		this->getChildByName("RightButton")->setVisible(true);
		if (iter == 0) this->getChildByName("LeftButton")->setVisible(false);
		if (iter == helpPageTotal - 1) this->getChildByName("RightButton")->setVisible(false);
	}
}
