#include "cocos2d.h"
#include "common.h"
USING_NS_CC;

bool isLand(const cocos2d::Node *node) {
	int tag = node->getTag();
	return tag == LAND_T || tag == SOFT_LAND_T || tag == SLIDING_LAND_T || tag == DROP_LAND_T ||
		tag == SWING_LAND_T || tag==TRAP_T || tag==SLIDING_TRAP_T;
}
bool isHero(const cocos2d::Node *node) {
	return node->getTag() == HERO_T;
}

bool touchUpSurface(const cocos2d::Node * nodeA, const cocos2d::Node * nodeB){
	//log("%f %f", nodeB->getBoundingBox().getMaxY(), nodeA->getBoundingBox().getMinY());
	//log("%d", (int)(nodeB->getBoundingBox().getMaxY() <= nodeA->getBoundingBox().getMinY() + 10.0f &&
	//	nodeB->getBoundingBox().getMaxY() >= nodeA->getBoundingBox().getMinY() - 10.0f));
	return (nodeB->getBoundingBox().getMaxY() <= nodeA->getBoundingBox().getMinY() + 10.0f &&
		nodeB->getBoundingBox().getMaxY() >= nodeA->getBoundingBox().getMinY() - 10.0f);
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
	}
}
