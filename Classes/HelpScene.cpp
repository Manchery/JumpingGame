#include "HelpScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include "ui/CocosGUI.h"

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

	auto optionBackGround = Sprite::create("ui/helpBackground.png");
	optionBackGround->setPosition(visibleSize / 2);
	optionBackGround->setContentSize(Size(visibleSize.width / 2, visibleSize.height*0.9));
	this->addChild(optionBackGround, -1);

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
	backButton->setPosition(visibleSize/2);
	this->addChild(backButton);

	return true;
}

