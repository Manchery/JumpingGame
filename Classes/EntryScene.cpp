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

static std::string chapterID[] = {
	"Chapter O:","Chapter 1:","Chapter 2:","Chapter 3:", "Chapter 4:","Chapter 5:","Bonus Chapter:"
};
static std::string chapterName[] = {
	"Knight and Castle","New-found Sword","Shield equipped","Into the Backyard", "Closest Opponent","Ultimate Battle","Fantastic Fair"
};
static int bonusNeed = 170;
static int getCoinTotal() {
	int total = 0;
	for (int i = 0; i < 7; i++) {
		total += UserDefault::getInstance()->getIntegerForKey(
			("chapter" + std::to_string(i) + "CoinCount").c_str());
	}
	return total;
}
static int getCurrentChapter() {
	for (int i = 0; i < 7; i++) {
		if (!UserDefault::getInstance()->getBoolForKey(
			("chapter" + std::to_string(i) + "Pass").c_str()))
			return i;
	}
	return 6;
}

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

	//back button

	int padding = 50;

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
	backButton->setPosition(Vec2(padding + backButton->getContentSize().width/2,
		visibleSize.height - padding - backButton->getContentSize().height / 2));
	this->addChild(backButton);

	// left-right button

	auto leftButton = Button::create("ui/leftTriangle.png");
	leftButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type) {
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED: {
			previousChapter();
		}break;
		default:
			break;
		}
	});
	leftButton->setName("LeftButton");
	leftButton->setPosition(Vec2(-visibleSize.width / 4 + visibleSize.width / 2 + 80+15, visibleSize.height / 2-50));
	this->addChild(leftButton,2);

	leftButton->setVisible(false);

	auto rightButton = Button::create("ui/rightTriangle.png");
	rightButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type) {
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED: {
			nextChapter();
		}break;
		default:
			break;
		}
	});
	rightButton->setName("RightButton");
	rightButton->setPosition(Vec2(visibleSize.width / 4 + visibleSize.width / 2 - 80+15, visibleSize.height / 2-50));
	this->addChild(rightButton,2);

	//dashboard
	
	auto coinScoreBoard = Node::create();
	std::stringstream sstr;  sstr << getCoinTotal() << " / " << 200;
	auto scoreLabel = Label::createWithTTF(sstr.str(), "fonts/GermaniaOne-Regular.ttf", 64);
	scoreLabel->setAnchorPoint(Vec2(1.0f, 1.0f)); scoreLabel->setPosition(Vec2::ZERO);

	auto coinLabel = Sprite::create("ui/gem.png");
	coinLabel->setContentSize(Size(72, 72));
	//auto label = Label::createWithTTF("Coin", "fonts/GermaniaOne-Regular.ttf", 64);
	coinLabel->setAnchorPoint(Vec2(1.0f, 1.0f));
	coinLabel->setPosition(Vec2(-(scoreLabel->getContentSize().width) - padding - 20, 0.0f));

	coinScoreBoard->addChild(coinLabel, 0);
	coinScoreBoard->addChild(scoreLabel, 0);
	coinScoreBoard->setAnchorPoint(Vec2(1.0f, 1.0f));
	coinScoreBoard->setPosition(Vec2(visibleSize.width - padding, visibleSize.height - padding));

	this->addChild(coinScoreBoard, 100);

	//chapter entry

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

	//key listener

	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(EntryScene::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(EntryScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	//bgm

	STOPBGM;

	return true;
}

Node * EntryScene::chapterEntry(int idx)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	int padding = 100;

	auto chapterString = "chapter" + std::to_string(idx);

	auto nodeItems = Node::create();

	auto size = Size((visibleSize.width) /2, visibleSize.height - padding *2);
	
	auto background = Sprite::create("ui/helpBackground.png");
	background->setContentSize(size);
	background->setPosition(Vec2(0, -80));
	nodeItems->addChild(background, 0);

	Vec2 orign = background->getPosition();

	auto snapshot = Sprite::create("snapshot/"+chapterString+".png");
	snapshot->setContentSize(Size(600,530));
	snapshot->setPosition(Vec2(0, -60));
	nodeItems->addChild(snapshot, 0);

	//title

	auto chapterIDLabel= Label::createWithTTF(chapterID[idx], "fonts/GermaniaOne-Regular.ttf", 96);
	auto chapterNameLabel = Label::createWithTTF(chapterName[idx], "fonts/GermaniaOne-Regular.ttf", 96);

	//coin

	auto coinItems = Node::create();

	std::stringstream sstr;
	sstr << UserDefault::getInstance()->getIntegerForKey((chapterString+"CoinCount").c_str())
		<<"/"<<chapterCoinTotal[idx];
	auto scoreLabel = Label::createWithTTF(sstr.str(), "fonts/GermaniaOne-Regular.ttf", 64);
	scoreLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
	scoreLabel->setPosition(Vec2(-40,0));

	auto coinLabel = Sprite::create("ui/gem.png");
	coinLabel->setContentSize(Size(72, 72));
	coinLabel->setAnchorPoint(Vec2(1.0f, 0.5f));
	coinLabel->setPosition(Vec2(-80,0));

	coinItems->addChild(scoreLabel);
	coinItems->addChild(coinLabel);

	//time

	auto timeItems = Node::create();

	char buf[100]; 
	auto recordTime = UserDefault::getInstance()->getIntegerForKey((chapterString + "Time").c_str());
	if (recordTime == INFTIME)
		sprintf(buf, "-- : --");
	else
		sprintf(buf, "%02d : %02d", recordTime / 60, recordTime % 60);
	auto timeLabel = Label::createWithTTF(buf, "fonts/GermaniaOne-Regular.ttf", 64);
	timeLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
	timeLabel->setPosition(Vec2(-40, 0));

	auto glasslabel = Sprite::create("ui/hourglass.png"); 
	glasslabel->setContentSize(Size(72, 72));
	glasslabel->setAnchorPoint(Vec2(1.0f, 0.5f));
	glasslabel->setPosition(Vec2(-80, 0));

	timeItems->addChild(timeLabel);
	timeItems->addChild(glasslabel);

	//

	padding = 30;

	chapterIDLabel->setPosition(orign+Vec2(0,size.height/2- padding-144));
	chapterNameLabel->setPosition(orign + Vec2(0, size.height / 2 - padding - 144 -padding -96));

	coinItems->setPosition(orign + Vec2(0, -size.height / 2+150+(padding+64)*2));
	timeItems->setPosition(orign + Vec2(0, -size.height / 2+150+padding+64));

	nodeItems->addChild(chapterIDLabel, 1);
	nodeItems->addChild(chapterNameLabel, 1);
	nodeItems->addChild(coinItems, 1);
	if (recordTime != INFTIME) nodeItems->addChild(timeItems, 1);

	//entry 
	
	if (idx <= getCurrentChapter() && (idx!=6 || getCoinTotal()>=bonusNeed)) {
		auto button = Button::create();
		button->ignoreContentAdaptWithSize(false);
		button->setContentSize(Size(size.width*0.8,size.height));
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
		locked->setContentSize(Size(256, 330)*0.5);
		locked->setPosition(snapshot->getPosition());
		nodeItems->addChild(locked, 2);
	}

	return nodeItems;
}

bool EntryScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	return true;
}
bool EntryScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE) {
		Director::getInstance()->replaceScene(HelloScene::createScene());
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
		nextChapter();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
		previousChapter();
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
		enterChapter(((Button*)chapters[iter]->getChildByName("Button"))->getTag());
	}
	return true;
}

void EntryScene::nextChapter()
{
	if (iter == chapterTotal - 1) return;
	chapters[iter]->setVisible(false);
	if (chapters[iter]->getChildByName("Button") != nullptr)
		((Button*)chapters[iter]->getChildByName("Button"))->setEnabled(false);
	if (iter < chapterTotal - 1)
		iter++;
	chapterPage = iter;
	chapters[iter]->setVisible(true);
	if (chapters[iter]->getChildByName("Button") != nullptr)
		((Button*)chapters[iter]->getChildByName("Button"))->setEnabled(true);
	
	this->getChildByName("LeftButton")->setVisible(true);
	this->getChildByName("RightButton")->setVisible(true);
	if (iter == 0) this->getChildByName("LeftButton")->setVisible(false);
	if (iter == chapterTotal - 1) this->getChildByName("RightButton")->setVisible(false);
}

void EntryScene::previousChapter()
{
	if (iter == 0) return;
	chapters[iter]->setVisible(false);
	if (chapters[iter]->getChildByName("Button") != nullptr)
		((Button*)chapters[iter]->getChildByName("Button"))->setEnabled(false);
	if (iter > 0)
		iter--;
	chapterPage = iter;
	chapters[iter]->setVisible(true);
	if (chapters[iter]->getChildByName("Button") != nullptr)
		((Button*)chapters[iter]->getChildByName("Button"))->setEnabled(true);

	this->getChildByName("LeftButton")->setVisible(true);
	this->getChildByName("RightButton")->setVisible(true);
	if (iter == 0) this->getChildByName("LeftButton")->setVisible(false);
	if (iter == chapterTotal - 1) this->getChildByName("RightButton")->setVisible(false);
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
