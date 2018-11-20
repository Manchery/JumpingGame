#include "TempScene.h"
Scene* TempScene::createScene()
{
	return TempScene::create();
}
bool TempScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	auto director = Director::getInstance();
	auto renderTexture = RenderTexture::create(director->getWinSize().width, director->getWinSize().height,
		Texture2D::PixelFormat::RGBA8888);
	renderTexture->getSprite()->setFlippedY(false);
	renderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
	director->getRunningScene()->visit();
	renderTexture->end();
	background = Sprite::createWithTexture(renderTexture->getSprite()->getTexture());
	background->setFlippedY(true);
	background->setAnchorPoint(Vec2::ZERO);
	this->addChild(background, -1);
	return true;
}