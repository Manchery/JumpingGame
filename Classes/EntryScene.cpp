#include "EntryScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "HelloScene.h"
#include "Chapter0Level1.h"
#include <iostream>
#include <sstream>

using namespace cocos2d::ui;
using namespace CocosDenshion;
USING_NS_CC;

Scene* EntryScene::createScene()
{
	return EntryScene::create();
}

bool EntryScene::init()
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

	auto backButton = Button::create("ui/buttonBackNormal.png", "ui/buttonBackSelected.png");
	backButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type) {
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED: {
			Director::getInstance()->replaceScene(HelloScene::createScene());
		}break;
		default:
			break;
		}
	});
	backButton->setPosition(Vec2(visibleSize.width-50-backButton->getContentSize().width/2,
		visibleSize.height - 50 - backButton->getContentSize().height / 2));
	this->addChild(backButton);

	auto node = chapterEntry(0);
	node->setPosition(visibleSize / 2);
	this->addChild(node, 1);

	return true;
}

static std::string chapterName[] = {
	"Chapter Zero","Chapter 1","Chapter 2","Chapter 3", "Chapter 4","Chapter Final","Chapter Bonus"
};
static int bonusNeed = 72;
static int getCoinTotal() {
	int total = 0;
	for (int i = 0; i < 6; i++) {
		std::stringstream sstr;
		sstr << "chapter" << i << "CoinCount";
		total+=UserDefault::getInstance()->getIntegerForKey(sstr.str().c_str());
	}
	return total;
}
static int getCurrentChapter() {
	for (int i = 0; i < 6; i++) {
		std::stringstream sstr;
		sstr << "chapter" << i << "Passed";
		if (!UserDefault::getInstance()->getBoolForKey(sstr.str().c_str()))
			return i;
	}
	return 6;
}

Node * EntryScene::chapterEntry(int idx)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	int paddingX = 20;

	std::stringstream sstr,sstr2;
	sstr << "chapter" << idx;
	auto nodeItems = Node::create();

	auto size = Size((visibleSize.width - paddingX * 2 - paddingX * 3) / 4, visibleSize.height / 4);
	
	auto background = Sprite::create("ui/helpBackground.png");
	background->setContentSize(size);
	background->setPosition(Vec2::ZERO);
	nodeItems->addChild(background, 0);

	auto snapshot = Sprite::create("snapshot/"+sstr.str()+".png");
	snapshot->setContentSize(Size(size.width*0.8,size.height*0.5));
	snapshot->setPosition(Vec2(0, -size.height*0.25 + size.height * 0.1));
	nodeItems->addChild(snapshot, 0);

	auto chapterLabel= Label::createWithTTF(chapterName[idx], "fonts/GermaniaOne-Regular.ttf", 64);
	sstr2 << "chapter" << idx << "CoinCount";
	sstr.str(""); sstr << "coin "<< UserDefault::getInstance()->getIntegerForKey(sstr2.str().c_str())
		<<"/"<<chapterCoinTotal[idx];
	auto coinLabel = Label::createWithTTF(sstr.str(), "fonts/GermaniaOne-Regular.ttf", 32);

	chapterLabel->setPosition(Vec2(0,size.height/2-20-32));
	coinLabel->setPosition(Vec2(0, size.height / 2 - 20 - 64-20-32));

	nodeItems->addChild(chapterLabel, 1);
	nodeItems->addChild(coinLabel, 1);
	
	if (idx <= getCurrentChapter() && (idx!=6 || getCoinTotal()>bonusNeed)) {
		auto button = Button::create();
		button->ignoreContentAdaptWithSize(false);
		button->setContentSize(size);
		button->setOpacity(0);
		button->setPosition(background->getPosition());
		button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
			switch (type) {
			case ui::Widget::TouchEventType::BEGAN:
				break;
			case ui::Widget::TouchEventType::ENDED: {
				Scene* scene;
				if (idx==0)
					scene = Chapter0Level1::createScene();
				else
					scene= Chapter0Level1::createScene();
				Director::getInstance()->replaceScene(scene);
			}break;
			default:
				break;
			}
		});

		nodeItems->addChild(button,2);
	}
	else {
		auto locked = Sprite::create("ui/lock.png");
		locked->setContentSize(Size(64, 64));
		locked->setPosition(snapshot->getPosition());
		nodeItems->addChild(locked, 2);
	}

	return nodeItems;
}

