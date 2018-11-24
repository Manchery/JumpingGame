#include "OptionScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "HelloScene.h"
#include <iostream>

using namespace CocosDenshion;
USING_NS_CC;
using namespace cocos2d::ui;

Scene* OptionScene::createScene()
{
	return OptionScene::create();
}

bool OptionScene::init()
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
	optionBackGround->setContentSize(Size(visibleSize.width / 2, visibleSize.height*0.8));
	this->addChild(optionBackGround, -1);

	//add the menu item for back to main menu
	auto label = LabelTTF::create("MainMenu", "Marker Felt.ttf", 32);
	auto menuItem = MenuItemLabel::create(label);
	menuItem->setCallback([&](cocos2d::Ref *sender) {
		Director::getInstance()->replaceScene(HelloScene::createScene());
	});

	auto menu = Menu::create(menuItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	menuItem->setPosition(Vec2(100,100));
	this->addChild(menu, 1);

	//add options
	addResolutionSet();
	addAudioSet();
	addResetOptions();
	addResetGame();

	return true;
}

// for resolution, need to restart to implement

static const std::string resolutionOption[] = {
	"small","medium","large","fullscreen"
};
void OptionScene::addResolutionSet() 
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
		this->addChild(resolutionCheckbox[i]);
	}
}

void OptionScene::addAudioSet(){
	Size visibleSize = Director::getInstance()->getVisibleSize();

	volumnSlider = Slider::create();
	volumnSlider->loadBarTexture("ui/Slider_Back.png");
	volumnSlider->loadSlidBallTextures("ui/SliderNode_Normal.png", "ui/SliderNode_Press.png", "ui/SliderNode_Disable.png");
	volumnSlider->loadProgressBarTexture("ui/Slider_PressBar.png");
	volumnSlider->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 10 * 40));

	volumnSlider->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED: {
			auto slider = (Slider*)sender;
			auto percent = slider->getPercent();
			log("slider percent = %d", percent);
			auto userData = UserDefault::getInstance();
			userData->setIntegerForKey("audio", percent);
			SimpleAudioEngine::getInstance()->setEffectsVolume(percent/100.0f);
			break;
		}
		default:
			break;
		}
	});

	volumnSlider->setPercent(UserDefault::getInstance()->getIntegerForKey("audio"));

	this->addChild(volumnSlider);
}

void OptionScene::addResetOptions()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto button = Button::create("ui/Button_Normal.png", "ui/Button_Press.png", "ui/Button_Disable.png");

	button->setTitleText("Reset options");
	button->setPosition(visibleSize/2);

	button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type){
			case ui::Widget::TouchEventType::BEGAN:
				break;
			case ui::Widget::TouchEventType::ENDED: {
				auto userData = UserDefault::getInstance();

				userData->setIntegerForKey("audio", 100);
				volumnSlider->setPercent(100);
				SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);

				userData->setStringForKey("resolution", "large");
				for (int i=0;i<4;i++)
					if (i==2)
						resolutionCheckbox[i]->setSelected(true);
					else
						resolutionCheckbox[i]->setSelected(false);

				log("reset options");
			}
				break;
			default:
				break;
		}
	});

	this->addChild(button);
}

void OptionScene::addResetGame()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto button = Button::create("ui/Button_Normal.png", "ui/Button_Press.png", "ui/Button_Disable.png");

	button->setTitleText("Reset Game");
	button->setTag(0);
	button->setPosition(Vec2(visibleSize.width/2,visibleSize.height/4));

	button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type) {
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED: {
			auto button = (Button*)sender;
			if (button->getTag() == 0) {
				button->loadTextureNormal("ui/Button_Normal_Sure.png");
				button->loadTexturePressed("ui/Button_Pressed_Sure.png");
				button->setTag(1);
				button->setTitleText("Sure?");
			}
			else {
				log("reset game");
				auto userData = UserDefault::getInstance();
				for (int i = 0; i <= 7; i++) {
					std::stringstream sstr;
					sstr << "chapter" << i << "CoinCount";
					userData->setIntegerForKey(sstr.str().c_str(), 0);
					sstr.str("");
					sstr << "chapter" << i << "Pass";
					userData->setBoolForKey(sstr.str().c_str(), false);
				}
				button->loadTextureNormal("ui/Button_Normal.png");
				button->loadTexturePressed("ui/Button_Pressed.png");
				button->setTag(0);
				button->setTitleText("Reset Game");
			}
		}
			break;
		default:
			break;
		}
	});

	this->addChild(button);
}
