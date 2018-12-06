#include "cocos2d.h"
#include "common.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;

float getMinYOffset(Sprite *sprite) {
	return sprite->getPhysicsBody()->getPositionOffset().y
		- ((PhysicsShapeBox*)(sprite->getPhysicsBody()->getShape(0)))->getSize().width / 2;
}
float getMaxYOffset(Sprite *sprite) {
	return sprite->getPhysicsBody()->getPositionOffset().y
		+ ((PhysicsShapeBox*)(sprite->getPhysicsBody()->getShape(0)))->getSize().width / 2;
}
float getMinXOffset(Sprite *sprite) {
	return sprite->getPhysicsBody()->getPositionOffset().x
		- ((PhysicsShapeBox*)(sprite->getPhysicsBody()->getShape(0)))->getSize().height / 2;
}
float getMaxXOffset(Sprite *sprite) {
	return sprite->getPhysicsBody()->getPositionOffset().x
		+ ((PhysicsShapeBox*)(sprite->getPhysicsBody()->getShape(0)))->getSize().height / 2;
}
float getPhysicsMinX(Sprite *sprite) {
	return sprite->getBoundingBox().getMidX() + getMinXOffset(sprite);
}
float getPhysicsMinY(Sprite *sprite) {
	return sprite->getBoundingBox().getMidY() + getMinYOffset(sprite);
}
float getPhysicsMaxX(Sprite *sprite) {
	return sprite->getBoundingBox().getMidX() + getMaxXOffset(sprite);
}
float getPhysicsMaxY(Sprite *sprite) {
	return sprite->getBoundingBox().getMidY() + getMaxYOffset(sprite);
}

bool isLand(const cocos2d::Node *node) {
	int tag = node->getTag();
	return tag == LAND_T || tag == SOFT_LAND_T || tag == SLIDING_LAND_T || tag == DROP_LAND_T ||
		tag == SWING_LAND_T || tag==TRAP_T || tag==SLIDING_TRAP_T || tag==ENEMY_T;
}
bool isTrap(const cocos2d::Node *node) {
	int tag = node->getTag();
	return tag == TRAP_T || tag == SLIDING_TRAP_T || tag == SWING_TRAP_T;
}
bool isHero(const cocos2d::Node *node) {
	return node->getTag() == HERO_T;
}

// Print useful error message instead of segfaulting when files are not there.
void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

void logUserDefault()
{
	auto userData = UserDefault::getInstance();
	log("save path %s", (userData->getXMLFilePath()).c_str());
	log("isExisted %d",(int)userData->getBoolForKey("isExisted")); 
	log("audio %d", userData->getIntegerForKey("audio"));
	log("resolution %s", (userData->getStringForKey("resolution")).c_str());
	for (int i = 0; i < 7; i++) {
		std::stringstream sstr;
		sstr << "chapter" << i << "CoinCount";
		log("%s %d", sstr.str().c_str(),userData->getIntegerForKey(sstr.str().c_str()));
		sstr.str("");
		sstr << "chapter" << i << "Pass";
		log("%s %d", sstr.str().c_str(), (int)(userData->getBoolForKey(sstr.str().c_str())));
		sstr.str("");
		sstr << "chapter" << i << "Time";
		log("%s %d", sstr.str().c_str(), userData->getIntegerForKey(sstr.str().c_str()));
	}
}

void setOptionDefault(){ 
	auto userData = UserDefault::getInstance();
	/*userData->setIntegerForKey("audio", DEFAULT_VOLUMN);
	SimpleAudioEngine::getInstance()->setEffectsVolume(DEFAULT_VOLUMN / 100.0f);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(DEFAULT_VOLUMN / 100.0f);*/
	userData->setStringForKey("resolution", DEFAULT_RESOLUTION);
}

void setGameDefault(){
	auto userData = UserDefault::getInstance();
	for (int i = 0; i < 7; i++) {
		std::stringstream sstr;
		sstr << "chapter" << i << "CoinCount";
		userData->setIntegerForKey(sstr.str().c_str(), 0);
		sstr.str("");
		sstr << "chapter" << i << "Pass";
		userData->setBoolForKey(sstr.str().c_str(), false);
		sstr.str("");
		sstr << "chapter" << i << "Time";
		userData->setIntegerForKey(sstr.str().c_str(), INFTIME);
	}
	userData->setBoolForKey("canShot", false);
	userData->setBoolForKey("canShield", false);
}
