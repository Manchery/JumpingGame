#include "EntryScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "HelloScene.h"
#include "Chapter0Level1.h"
#include "Chapter1Level1.h"
#include "Chapter2Level1.h"
#include "Chapter3Level1.h"
#include "Chapter3Level2.h"
#include "Chapter3Level3.h"
#include "Chapter4Level1.h"
#include "Chapter5Level1.h"
#include "Chapter6Level1.h"
#include "common.h"
#include <iostream>
#include <sstream>

using namespace cocos2d::ui;
using namespace CocosDenshion;
USING_NS_CC;

int EntryScene::chapterPage = 0;

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

	chapterTotal = 7;

	for (int i = 0; i < chapterTotal; i++) {
		auto chapter = chapterEntry(i); chapter->retain();
		chapter->setPosition(Vec2(visibleSize.width / 2 , visibleSize.height / 2));
		chapter->setVisible(false);
		if (chapter->getChildByName("Button") != nullptr)
			((Button*)chapter->getChildByName("Button"))->setEnabled(false);
		this->addChild(chapter, 1);
		chapters.push_back(chapter);
	}
	iter = chapterPage;
	chapters[iter]->setVisible(true);
	if (chapters[iter]->getChildByName("Button") != nullptr)
		((Button*)chapters[iter]->getChildByName("Button"))->setEnabled(true);

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(EntryScene::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(EntryScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	STOPBGM;

	return true;
}

static std::string chapterName[] = {
	"Chapter Zero","Chapter 1","Chapter 2","Chapter 3", "Chapter 4","Chapter Final","Chapter Bonus"
};
static int bonusNeed = 170;
static int getCoinTotal() {
	int total = 0;
	for (int i = 0; i < 6; i++) {
		total+=UserDefault::getInstance()->getIntegerForKey(
			("chapter" + std::to_string(i) + "CoinCount").c_str());
	}
	return total;
}
static int getCurrentChapter() {
	for (int i = 0; i < 6; i++) {
		if (!UserDefault::getInstance()->getBoolForKey(
			("chapter" + std::to_string(i) + "Pass").c_str()))
			return i;
	}
	return 6;
}

Node * EntryScene::chapterEntry(int idx)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	int padding = 200;

	auto chapterString = "chapter" + std::to_string(idx);

	auto nodeItems = Node::create();

	auto size = Size((visibleSize.width) /2, visibleSize.height - padding *2);
	
	auto background = Sprite::create("ui/helpBackground.png");
	background->setContentSize(size);
	background->setPosition(Vec2::ZERO);
	nodeItems->addChild(background, 0);

	auto snapshot = Sprite::create("snapshot/"+chapterString+".png");
	snapshot->setContentSize(Size(size.width*0.8,size.height*0.5));
	snapshot->setPosition(Vec2(0, -size.height*0.25 + size.height * 0.1));
	nodeItems->addChild(snapshot, 0);

	auto chapterLabel= Label::createWithTTF(chapterName[idx], "fonts/GermaniaOne-Regular.ttf", 128);

	std::stringstream sstr;
	sstr << "coin "<< UserDefault::getInstance()->getIntegerForKey((chapterString+"CoinCount").c_str())
		<<"/"<<chapterCoinTotal[idx];
	auto coinLabel = Label::createWithTTF(sstr.str(), "fonts/GermaniaOne-Regular.ttf", 64);

	char buf[100]; 
	auto recordTime = UserDefault::getInstance()->getIntegerForKey((chapterString + "Time").c_str());
	if (recordTime == INFTIME)
		sprintf(buf, "-- : --");
	else
		sprintf(buf, "%02d : %02d", recordTime / 60, recordTime % 60);
	auto timeLabel = Label::createWithTTF(buf, "fonts/GermaniaOne-Regular.ttf", 64);

	padding = 30;

	chapterLabel->setPosition(Vec2(0,size.height/2- padding-128));
	coinLabel->setPosition(Vec2(0, size.height / 2 - padding - 128- padding -64));
	timeLabel->setPosition(Vec2(0, size.height / 2 - padding - 128 - padding - 64 - padding - 64));

	nodeItems->addChild(chapterLabel, 1);
	nodeItems->addChild(coinLabel, 1);
	nodeItems->addChild(timeLabel, 1);
	
	if (idx <= getCurrentChapter() && (idx!=6 || getCoinTotal()>=bonusNeed)) {
		auto button = Button::create();
		button->ignoreContentAdaptWithSize(false);
		button->setContentSize(size);
		button->setOpacity(0);
		button->setTag(idx);
		button->setName("Button");
		button->setPosition(background->getPosition());
		button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
			switch (type) {
			case ui::Widget::TouchEventType::BEGAN:
				break;
			case ui::Widget::TouchEventType::ENDED: {
				enterChapter(((Button*)sender)->getTag());
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

void EntryScene::enterChapter(int idx) {
	Scene* scene;
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
	//Director::getInstance()->replaceScene(scene);
}

bool EntryScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	return true;
}
bool EntryScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	
	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
		chapters[iter]->setVisible(false);
		if (chapters[iter]->getChildByName("Button") != nullptr)
			((Button*)chapters[iter]->getChildByName("Button"))->setEnabled(false);
		if (iter < chapterTotal - 1) 
			iter++;
		chapterPage = iter;
		chapters[iter]->setVisible(true);
		if (chapters[iter]->getChildByName("Button") != nullptr)
			((Button*)chapters[iter]->getChildByName("Button"))->setEnabled(true);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
		chapters[iter]->setVisible(false);
		if (chapters[iter]->getChildByName("Button") != nullptr)
			((Button*)chapters[iter]->getChildByName("Button"))->setEnabled(false);
		if (iter > 0)
			iter--;
		chapterPage = iter;
		chapters[iter]->setVisible(true);
		if (chapters[iter]->getChildByName("Button") != nullptr)
			((Button*)chapters[iter]->getChildByName("Button"))->setEnabled(true);
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
		enterChapter(((Button*)chapters[iter]->getChildByName("Button"))->getTag());
	}
	
	return true;
}
