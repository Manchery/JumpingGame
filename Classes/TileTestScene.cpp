#include "TileTestScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include <algorithm>

USING_NS_CC;
using namespace CocosDenshion;

Scene* TileTestScene::createScene()
{
	//return GameScene::createWithPhysics();
	return TileTestScene::create();
}

void TileTestScene::drawBackGround(ValueVector &arrObj,int zOrder=0) {
	for (auto object : arrObj)
	{
		auto dic = object.asValueMap();
		std::string type = dic.at("type").asString();
		float width = dic.at("width").asFloat();
		float height = dic.at("height").asFloat();
		float x = dic.at("x").asFloat();
		float y = dic.at("y").asFloat() + height;
		float rotation = dic.at("rotation").asFloat();
		bool flippedX = (dic.at("gid").asFloat()>2e9);

		if (type != "") {
			auto sprite = Sprite::create("map/" + type + ".png");
			sprite->setContentSize(Size(width, height));
			sprite->setAnchorPoint(Vec2::ZERO);
			sprite->setPosition(x, y);
			if (rotation!=0)
				sprite->setRotation(rotation);
			if (flippedX)
				sprite->setFlippedX(true);
			if (type == "shadow")
				sprite->setOpacity(64);
			backGroundLayer->addChild(sprite, zOrder);
		}
	}
}
void TileTestScene::drawMap(ValueVector &arrObj, int zOrder = 0) {
	for (auto object : arrObj)
	{
		auto dic = object.asValueMap();
		std::string type = dic.at("type").asString();
		std::string name = dic.at("name").asString();
		float width = dic.at("width").asFloat();
		float height = dic.at("height").asFloat();
		float x = dic.at("x").asFloat();
		float y = dic.at("y").asFloat() + height;

		bool flippedX = (dic.at("gid").asFloat() > 2e9);
		
		if (type == "") continue;

		if (name == "Land") {
			auto sprite = Sprite::create("map/" + type + ".png");
			sprite->setContentSize(Size(width, height));
			sprite->setAnchorPoint(Vec2::ZERO);
			sprite->setPosition(x, y);
			if (flippedX)
				sprite->setFlippedX(true);
			frontGroundLayer->addChild(sprite, zOrder);
		}
		else if (name == "Hero") {

		}

	}
}
bool TileTestScene::init() {

	if (!Scene::init())
		return false;
	//xml

	auto tileMap = TMXTiledMap::create("map/chapter0.tmx");

	auto mapSize = Size(tileMap->getMapSize().width*tileMap->getTileSize().width,
		tileMap->getMapSize().height*tileMap->getTileSize().height);

	frontGroundLayer = Layer::create();
	//frontGroundLayer->setContentSize(mapSize);
	this->addChild(frontGroundLayer, 0);

	backGroundLayer = Layer::create();
	//backGroundLayer->setContentSize(mapSize);
	this->addChild(backGroundLayer, -1);

	backGroundLayer->addChild(LayerColor::create(Color4B::Color4B(39, 185, 154,255)), -1);

	auto objGroups=tileMap->getObjectGroups();
	for (auto objGroup : objGroups) {
		if (objGroup->getGroupName()!="game")
			drawBackGround(objGroup->getObjects());
	}
	drawMap(tileMap->getObjectGroup("game")->getObjects());
	return true;
}