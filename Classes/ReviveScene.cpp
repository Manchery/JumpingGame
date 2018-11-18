#include "ReviveScene.h"
#include "SimpleAudioEngine.h"
#include "common.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* ReviveScene::createScene()
{
	return ReviveScene::create();
}
bool ReviveScene::init()
{
	if (!TempScene::init())
	{
		return false;
	}
	auto tintTo1 = TintTo::create(0.1f, 120.0f, 232.0f, 254.0f);
	auto tintTo2 = TintTo::create(0.1f, 232.0f, 120.0f, 254.0f);
	auto tintTo3 = TintTo::create(0.1f, 232.0f, 254.0f, 120.0f);
	auto delay = DelayTime::create(0.2);
	auto seq = Sequence::create(delay,tintTo1,delay->clone(),tintTo2,delay->clone(),tintTo3,nullptr);
	background->runAction(seq);

	if (AUDIO_PLAY)
		SimpleAudioEngine::getInstance()->playEffect("parkour_sounds/gameover.wav", false, 1.0f, 1.0f, 1.0f);

	this->scheduleOnce(schedule_selector(ReviveScene::goBack),1.0f);
	return true;
}

void ReviveScene::goBack(float dt) {
	Director::getInstance()->popScene();
}