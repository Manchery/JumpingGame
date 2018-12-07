#include "SimpleAudioEngine.h"
#include "LoadingScene.h"
#include "EntryScene.h"
#include "HelloScene.h"

USING_NS_CC;
using namespace CocosDenshion;

static const std::vector<std::string> mapImages = {
"alter.png",
"backgroundArch.png",
"backgroundMountain.png",
"backgroundTower.png",
"backgroundTree.png",
"blobBlue.png",
"blobGreen.png",
"blue.png",
"bombStroked.png",
"boss.png",
"castleWall.png",
"cloud.png",
"column1.png",
"column2.png",
"doorBlueStroked.png",
"doorGreenStroke.png",
"doorRedStroked.png",
"doorStroked.png",
"earthWall.png",
"earthWall2.png",
"exit.png",
"flare.png",
"gemBlueStroked.png",
"gemRedStroked.png",
"grassLarge.png",
"grassSmall.png",
"grey.png",
"hero.png",
"keyGreenStroked.png",
"keyRedStroked.png",
"keyYellowStroked.png",
"platform1.png",
"platform2.png",
"platform3.png",
"platform4.png",
"platformBase1.png",
"platformBase2.png",
"platformBase3.png",
"platformBase4.png",
"platformBlock1.png",
"platformBlock2.png",
"platformBlock3.png",
"platformBlock4.png",
"platformConnector1.png",
"platformConnector2.png",
"platformConnector3.png",
"platformConnector4.png",
"pushBlock1.png",
"pushBlock2.png",
"pushBlock3.png",
"row1.png",
"row2.png",
"shadow.png",
"shieldStroked.png",
"sign.png",
"sign2.png",
"skeleton.png",
"swordStroked.png",
"torch.png",
"trap.png",
"trapSmall.png",
"trapTiny.png",
"wallDecor1.png",
"wallDecor2.png",
"wallDecor3.png",
"window1.png",
"window2.png",
"window3.png"
};
static const std::vector<std::string> uiImages = {
"backgroundSet.png",
"bitmapRed.png",
"block.png",
"buttonBackNormal.png",
"buttonBackSelected.png",
"buttonExitNormal.png",
"buttonExitSelected.png",
"buttonHelpNormal.png",
"buttonHelpSelected.png",
"buttonOptionNormal.png",
"buttonOptionSelected.png",
"buttonResetGameNormal.png",
"buttonResetGameSelected.png",
"buttonResetOptionNormal.png",
"buttonResetOptionSelected.png",
"buttonRestartNormal.png",
"buttonRestartSelected.png",
"buttonResumeNormal.png",
"buttonResumeSelected.png",
"buttonStartNormal.png",
"buttonStartSelected.png",
"buttonSureNormal.png",
"buttonSureSelected.png",
"CheckBoxNode_Disable.png",
"CheckBoxNode_Normal.png",
"CheckBox_Disable.png",
"CheckBox_Normal.png",
"CheckBox_Press.png",
"cloud.png",
"gem.png",
"heart.png",
"helloground.png",
"helpBackground.png",
"helppage1.png",
"helppage2.png",
"helppage3.png",
"helppage4.png",
"helppage5.png",
"LoadingBarFile.png",
"lock.png",
"messageBox.png",
"shield.png",
"SliderNode_Disable.png",
"SliderNode_Normal.png",
"SliderNode_Press.png",
"sliderProgress.png",
"sliderTrack.png",
"Slider_Back.png",
"Slider_PressBar.png",
"title.png"
};
static const std::vector<std::string> heroImages = {
	"boss.png",
"bullet.png",
"EnemyLeft1.png",
"EnemyLeft2.png",
"EnemyLeft3.png",
"EnemyLeft4.png",
"EnemyLeftSilence.png",
"EnemyRight1.png",
"EnemyRight2.png",
"EnemyRight3.png",
"EnemyRight4.png",
"EnemyRightSilence.png",
"firework.png",
"HeroJump.png",
"HeroJumpProtected.png",
"HeroJumpShield.png",
"HeroJumpShot.png",
"HeroLeft1.png",
"HeroLeft1Protected.png",
"HeroLeft1Shield.png",
"HeroLeft1Shot.png",
"HeroLeft2.png",
"HeroLeft2Protected.png",
"HeroLeft2Shield.png",
"HeroLeft2Shot.png",
"HeroLeft3.png",
"HeroLeft3Protected.png",
"HeroLeft3Shield.png",
"HeroLeft3Shot.png",
"HeroLeft4.png",
"HeroLeft4Protected.png",
"HeroLeft4Shield.png",
"HeroLeft4Shot.png",
"HeroLeftJump.png",
"HeroLeftJumpProtected.png",
"HeroLeftJumpShield.png",
"HeroLeftJumpShot.png",
"HeroLeftSilence.png",
"HeroLeftSilenceProtected.png",
"HeroLeftSilenceShield.png",
"HeroLeftSilenceShot.png",
"HeroRight1.png",
"HeroRight1Protected.png",
"HeroRight1Shield.png",
"HeroRight1Shot.png",
"HeroRight2.png",
"HeroRight2Protected.png",
"HeroRight2Shield.png",
"HeroRight2Shot.png",
"HeroRight3.png",
"HeroRight3Protected.png",
"HeroRight3Shield.png",
"HeroRight3Shot.png",
"HeroRight4.png",
"HeroRight4Protected.png",
"HeroRight4Shield.png",
"HeroRight4Shot.png",
"HeroRightJump.png",
"HeroRightJumpProtected.png",
"HeroRightJumpShield.png",
"HeroRightJumpShot.png",
"HeroRightSilence.png",
"HeroRightSilenceProtected.png",
"HeroRightSilenceShield.png",
"HeroRightSilenceShot.png",
"lightRing.png"
};
static const std::vector<std::string> snapshotImages = {
	"chapter0.png","chapter1.png","chapter2.png","chapter3.png","chapter4.png","chapter5.png","chapter6.png"
};

Scene* LoadingScene::createScene()
{
	return LoadingScene::create();
}

bool LoadingScene::init()
{
	if (!Scene::init())
		return false;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();

	auto backGround = Sprite::create("ui/helloground.png");
	backGround->setContentSize(visibleSize);
	backGround->setAnchorPoint(Vec2::ZERO);
	backGround->setPosition(Vec2::ZERO);
	this->addChild(backGround, -1);

	auto title = Sprite::create("ui/title.png");
	title->setContentSize(title->getContentSize()*2.0f);
	title->setPosition(visibleSize.width / 2, visibleSize.height / 4 * 3);
	this->addChild(title, 1);

	totalNum = uiImages.size()+mapImages.size() + heroImages.size()+snapshotImages.size();
	loadedNum = 0;

	Sprite *progressBkground = Sprite::create("ui/sliderTrack.png"); 
	Size progressBackSize = progressBkground->getContentSize();
	progressBkground->setPosition(visibleSize.width / 2, visibleSize.height / 3);
	this->addChild(progressBkground,2);
	Sprite *progressBkground2 = Sprite::create("ui/sliderBackground.png");
	progressBkground2->setPosition(visibleSize.width / 2, visibleSize.height / 3);
	this->addChild(progressBkground2, 0);

	loadProgress = ProgressTimer::create(Sprite::create("ui/sliderProgress.png"));
	loadProgress->setBarChangeRate(Vec2(1, 0)); 
	loadProgress->setType(ProgressTimer::Type::BAR); 
	loadProgress->setMidpoint(Vec2(0, 1)); 
	Size progressSize = loadProgress->getContentSize();
	loadProgress->setPosition(visibleSize.width / 2, visibleSize.height / 3);
	loadProgress->setPercentage(0.0f);
	this->addChild(loadProgress,1);

	auto loadingLabel = Label::createWithTTF("Loading", "fonts/GermaniaOne-Regular.ttf", 72);
	loadingLabel->setPosition(visibleSize.width / 2, visibleSize.height / 3+128);
	this->addChild(loadingLabel);

	return true;
}

void LoadingScene::onEnterTransitionDidFinish()
{
	for (auto uiImage : uiImages) {
		Director::getInstance()->getTextureCache()->addImageAsync("ui/"+uiImage, CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	}
	for (auto mapImage : mapImages) {
		Director::getInstance()->getTextureCache()->addImageAsync("map/" + mapImage, CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	}
	for (auto heroImage : heroImages) {
		Director::getInstance()->getTextureCache()->addImageAsync("hero/" + heroImage, CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	}
	for (auto snapshotImage : snapshotImages) {
		Director::getInstance()->getTextureCache()->addImageAsync("snapshot/" + snapshotImage, CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
	}
}

void LoadingScene::loadingCallBack(Texture2D *texture)
{
	loadedNum++;
	loadProgress->setPercentage((float)loadedNum / totalNum * 100);

	if (loadedNum == totalNum)
	{
		/*auto boyAnimation = Animation::create();
		boyAnimation->setDelayPerUnit(0.1f);
		for (int i = 1; i <= 12; i++)
		{
			char str[100] = { 0 };
			sprintf(str, "boy%d.png", i);
			boyAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(str));
		}

		AnimationCache::getInstance()->addAnimation(boyAnimation, "boyAnimation");

		auto girlAnimation = Animation::create();
		girlAnimation->setDelayPerUnit(0.2f);
		for (int i = 1; i <= 8; i++)
			girlAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("girl" + std::to_string(i) + ".png"));
		AnimationCache::getInstance()->addAnimation(girlAnimation, "girlAnimation");

		SimpleAudioEngine::getInstance()->preloadBackgroundMusic("spring_music.wav");
		SimpleAudioEngine::getInstance()->preloadBackgroundMusic("winter_music.mp3");

		SimpleAudioEngine::getInstance()->preloadEffect("jump.wav");
		SimpleAudioEngine::getInstance()->preloadEffect("point.mp3");
		SimpleAudioEngine::getInstance()->preloadEffect("gameover.wav");*/

		auto helloScene = HelloScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, helloScene));
	}

}