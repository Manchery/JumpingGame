#include "OptionScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "HelloScene.h"
#include "common.h"
#include <iostream>

using namespace CocosDenshion;
USING_NS_CC;
using namespace cocos2d::ui;

Scene* OptionScene::createScene(bool canResetGame)
{
	OptionScene *scene = new (std::nothrow) OptionScene();
	if (scene && scene->init(canResetGame))
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

bool OptionScene::init(bool canResetGame)
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

	auto optionBackGround=Sprite::create("ui/helpBackground.png");
	optionBackGround->setPosition(visibleSize / 2);
	optionBackGround->setContentSize(Size(visibleSize.width / 2, visibleSize.height*0.9));
	this->addChild(optionBackGround, -1);

	//init options
	initResolutionSet();
	//initAudioSet();
	initResetOption();
	initResetGame();

	//back to main scene

	backButton = Button::create("ui/buttonBackNormal.png", "ui/buttonBackSelected.png");
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

	auto sy = (optionBackGround->getContentSize().height - 40 * 2) / 14;

	auto menuNode = Node::create();

	auto titleLabel = Label::createWithTTF("OPTION", "fonts/GermaniaOne-Regular.ttf", 128);
	titleLabel->setPosition(Vec2(0,6*sy));
	menuNode->addChild(titleLabel);

	auto resolutionLabel= Label::createWithTTF("Resolution", "fonts/GermaniaOne-Regular.ttf", 72);
	resolutionLabel->setAnchorPoint(Vec2(0, 0.5));
	resolutionLabel->setPosition(Vec2(-optionBackGround->getContentSize().width / 2 + 120, 4.5*sy));
	menuNode->addChild(resolutionLabel);

	auto smallLabel = Label::createWithTTF("480 x 320", "fonts/GermaniaOne-Regular.ttf", 72);
	smallLabel->setAnchorPoint(Vec2(0, 0.5));
	smallLabel->setPosition(Vec2(-60, 4.5*sy));
	menuNode->addChild(smallLabel);
	resolutionCheckbox[0]->setPosition(Vec2(optionBackGround->getContentSize().width / 2 - 160,4.5*sy));
	menuNode->addChild(resolutionCheckbox[0]);

	auto mediumLabel = Label::createWithTTF("1024 x 768", "fonts/GermaniaOne-Regular.ttf", 72);
	mediumLabel->setAnchorPoint(Vec2(0, 0.5));
	mediumLabel->setPosition(Vec2(-60, 3.5*sy));
	menuNode->addChild(mediumLabel);
	resolutionCheckbox[1]->setPosition(Vec2(optionBackGround->getContentSize().width / 2 - 160, 3.5*sy));
	menuNode->addChild(resolutionCheckbox[1]);

	auto largeLabel = Label::createWithTTF("2048 x 1536", "fonts/GermaniaOne-Regular.ttf", 72);
	largeLabel->setAnchorPoint(Vec2(0, 0.5));
	largeLabel->setPosition(Vec2(-60, 2.5*sy));
	menuNode->addChild(largeLabel);
	resolutionCheckbox[2]->setPosition(Vec2(optionBackGround->getContentSize().width / 2 - 160, 2.5*sy));
	menuNode->addChild(resolutionCheckbox[2]);

	auto fullscreenLabel = Label::createWithTTF("Full Screen", "fonts/GermaniaOne-Regular.ttf", 72);
	fullscreenLabel->setAnchorPoint(Vec2(0, 0.5));
	fullscreenLabel->setPosition(Vec2(-60, 1.5*sy));
	menuNode->addChild(fullscreenLabel);
	resolutionCheckbox[3]->setPosition(Vec2(optionBackGround->getContentSize().width / 2 - 160, 1.5*sy));
	menuNode->addChild(resolutionCheckbox[3]);

	/*auto volumnLabel = Label::createWithTTF("Volume", "fonts/GermaniaOne-Regular.ttf", 72);
	volumnLabel->setAnchorPoint(Vec2(0, 0.5));
	volumnLabel->setPosition(Vec2(-optionBackGround->getContentSize().width / 2 + 120, 0.5*sy));
	menuNode->addChild(volumnLabel);
	volumnSlider->setAnchorPoint(Vec2(0, 0.5));
	volumnSlider->setPosition(Vec2(-60, 0.5*sy));
	menuNode->addChild(volumnSlider);*/

	std::stringstream sstr;
	sstr << "User Data are saved in\n" << UserDefault::getInstance()->getXMLFilePath()<<"\n";
	sstr << "To implement resolution option, you need to restart.";
	auto hintLabel = Label::createWithTTF(sstr.str(), "fonts/GermaniaOne-Regular.ttf", 24, 
		Size::ZERO,TextHAlignment::CENTER);
	hintLabel->setPosition(Vec2(0,0.5*sy));
	menuNode->addChild(hintLabel);

	if (canResetGame) {
		resetOptionButton->setPosition(Vec2(0, -1 * sy - sy / 4));
		menuNode->addChild(resetOptionButton);

		resetGameButton->setPosition(Vec2(0, -2 * sy - sy / 2));
		menuNode->addChild(resetGameButton);

		backButton->setPosition(Vec2(0, -3 * sy - sy));
		menuNode->addChild(backButton);
	}
	else {
		resetOptionButton->setPosition(Vec2(0, -1 * sy - sy / 4));
		menuNode->addChild(resetOptionButton);

		backButton->setPosition(Vec2(0, -2 * sy - sy));
		menuNode->addChild(backButton);
	}

	menuNode->setPosition(visibleSize / 2);
	this->addChild(menuNode, 1);

	return true;
}

// for resolution, need to restart to implement

static const std::string resolutionOption[] = {
	"small","medium","large","fullscreen"
};
void OptionScene::initResolutionSet()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	for (int i = 0; i < 4; i++) {
		resolutionCheckbox[i] = CheckBox::create("ui/CheckBox_Normal.png",
			"ui/CheckBox_Press.png",
			"ui/CheckBoxNode_Normal.png",
			"ui/CheckBox_Disable.png",
			"ui/CheckBoxNode_Disable.png");
		resolutionCheckbox[i]->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - (i + 5) * 40));
		resolutionCheckbox[i]->setTag(i);
		resolutionCheckbox[i]->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
			
			switch (type) {
			case ui::Widget::TouchEventType::BEGAN:
				break;
			case ui::Widget::TouchEventType::ENDED: {
				auto checkbox = (CheckBox*)sender;
				int tag = checkbox->getTag();
				auto userData = UserDefault::getInstance();
				log("resolution changed to %s", resolutionOption[tag].c_str());
				userData->setStringForKey("resolution", resolutionOption[tag]);
				for (int i = 0; i < 4; i++)
					if (i != tag)
						resolutionCheckbox[i]->setSelected(false);
			}
				break;
			default:
				break;
			}
		});
		if (UserDefault::getInstance()->getStringForKey("resolution") == resolutionOption[i])
			resolutionCheckbox[i]->setSelected(true);
		//this->addChild(resolutionCheckbox[i]);
	}
}

//void OptionScene::initAudioSet(){
//	Size visibleSize = Director::getInstance()->getVisibleSize();
//
//	volumnSlider = Slider::create();
//	volumnSlider->loadBarTexture("ui/Slider_Back.png");
//	volumnSlider->loadSlidBallTextures("ui/SliderNode_Normal.png", "ui/SliderNode_Press.png", "ui/SliderNode_Disable.png");
//	volumnSlider->loadProgressBarTexture("ui/Slider_PressBar.png");
//	volumnSlider->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 10 * 40));
//
//	volumnSlider->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
//		switch (type)
//		{
//		case ui::Widget::TouchEventType::BEGAN:
//			break;
//		case ui::Widget::TouchEventType::ENDED: {
//			auto slider = (Slider*)sender;
//			auto percent = slider->getPercent();
//			log("slider percent = %d", percent);
//			auto userData = UserDefault::getInstance();
//			userData->setIntegerForKey("audio", percent);
//			SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(percent/100.0f);
//			SimpleAudioEngine::getInstance()->setEffectsVolume(percent / 100.0f);
//			break;
//		}
//		default:
//			break;
//		}
//	});
//
//	volumnSlider->setPercent(UserDefault::getInstance()->getIntegerForKey("audio"));
//
//	//this->addChild(volumnSlider);
//}

void OptionScene::initResetOption()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	resetOptionButton = Button::create("ui/buttonResetOptionNormal.png", "ui/buttonResetOptionSelected.png");

	//button->setTitleText("Reset options");
	resetOptionButton->setPosition(visibleSize/2);

	resetOptionButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type){
			case ui::Widget::TouchEventType::BEGAN:
				break;
			case ui::Widget::TouchEventType::ENDED: {
				setOptionDefault();
				//volumnSlider->setPercent(DEFAULT_VOLUMN);
				for (int i=0;i<4;i++)
					if (resolutionOption [i]==DEFAULT_RESOLUTION)
						resolutionCheckbox[i]->setSelected(true);
					else
						resolutionCheckbox[i]->setSelected(false);
				//log("reset options");
			}break;
			default:
				break;
		}
	});

	//this->addChild(button);
}

void OptionScene::initResetGame()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	resetGameButton = Button::create("ui/buttonResetGameNormal.png", "ui/buttonResetGameSelected.png");

	//button->setTitleText("Reset Game");
	resetGameButton->setTag(0);
	resetGameButton->setPosition(Vec2(visibleSize.width/2,visibleSize.height/4));

	resetGameButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type) {
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED: {
			auto button = (Button*)sender;
			if (button->getTag() == 0) {
				button->loadTextureNormal("ui/buttonSureNormal.png");
				button->loadTexturePressed("ui/buttonSureSelected.png");
				button->setTag(1);
				//button->setTitleText("Sure?");
			}
			else {
				//log("reset game");
				setGameDefault();
				button->loadTextureNormal("ui/buttonResetGameNormal.png");
				button->loadTexturePressed("ui/buttonResetGameSelected.png");
				button->setTag(0);
				//button->setTitleText("Reset Game");
			}
		}break;
		default:
			break;
		}
	});

	//this->addChild(button);
}
