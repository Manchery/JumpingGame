#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Hero.h"
#include "Enemy.h"
#include "FollowEnemy.h"
#include "common.h"
#include "SwingLand.h"

USING_NS_CC;

class GameScene : public cocos2d::Scene
{
public:
	virtual void onEnterTransitionDidFinish();
	static cocos2d::Scene* createScene();
	void commonInitAfterMap();
	virtual bool init();
	virtual void initMap(const std::string & tmxFile, const Color4B & backgroundColor);
	virtual void initListener();
	virtual void initDashboard();
	virtual void initBackgroundMusic();

	virtual bool onContactBegin(cocos2d::PhysicsContact &contact);
	bool onContactPreSolve(PhysicsContact& contact, PhysicsContactPreSolve& solve);
	bool onContactPostSolve(PhysicsContact& contact, const PhysicsContactPostSolve& solve);
	bool onContactEnd(cocos2d::PhysicsContact &contact);
	bool onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	bool onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void heroUpdate(float dt);
	virtual void mapUpdate(float dt);
	virtual void regenerateUpdate(float dt);
	virtual void messageUpdate(float dt);
	virtual void timerUpdate(float dt);

	void setRevivePoint(Vec2 revive);

	void addRenerate(Node * node);

	Vec2 getHeroGlobalPosition();

	void gamePause();
	virtual void heroDie();
	void heroJump();

	void messageSingleLine(const std::string &mes);
	void messageDoubleLine(const std::string &mes1,const std::string &mes2);
	void backToEntry(float dt);
	void win();
	void nextLevel(GameScene *scene);
	virtual void gamePass();

	void drawBackGround(ValueVector & arrObj, int zOrder);

	virtual void drawMap(const TMXTiledMap * tileMap);

	CREATE_FUNC(GameScene);
	PhysicsBody* getBoundBody() {
		return frontGroundLayer->getChildByTag(BORDER_T)->getPhysicsBody();
	}
	Hero* getHero() {
		return hero;
	}
	void addToFrontGround(Node *node,int zOrder=1){
		frontGroundLayer->addChild(node, zOrder);
	}
	void setCoinCount(int count) {
		coinCount = count;
	}
	float getRunningTime() {
		return runningTime;
	}
	void setRunningTime(float rtime) {
		runningTime = rtime;
		((Label*)(this->getChildByName("TimeLabel")))->setString(std::to_string((int)(runningTime)));
	}
	bool getGotShot() {
		return gotShot;
	}
	bool getGotShield() {
		return gotShield;
	}
	bool heroBottomTouched(Node *node) {
		return (node->getBoundingBox().getMaxY() <= hero->getPhysicsMinY() + 10.0f &&
			node->getBoundingBox().getMaxY() >= hero->getPhysicsMinY() - 10.0f);
	}
	bool heroRightTouched(Node *node) {
		return (node->getBoundingBox().getMinX() <= hero->getPhysicsMaxX() + 10.0f &&
			node->getBoundingBox().getMinX() >= hero->getPhysicsMaxX() - 10.0f);
	}
	bool heroLeftTouched(Node *node) {
		return (node->getBoundingBox().getMaxX() <= hero->getPhysicsMinX() + 10.0f &&
			node->getBoundingBox().getMaxX() >= hero->getPhysicsMinX() - 10.0f);
	}

protected:
	Layer *frontGroundLayer, *backGroundLayer;
	std::vector<Node*> destroyedList;
	std::vector<std::pair<Node*, float> > regenList;
	Size mapSize;
	Hero *hero;
	FollowEnemy *followEnemy;
	bool upKeyDown, leftKeyDown, rightKeyDown, downKeyDown;
	cocos2d::EventKeyboard::KeyCode lastKey;
	Vec2 revivePoint;
	bool heroDied, heroJumped,heroBounced;
	int coinCount,coinTotal;
	bool needGameKey, gotGameKey;
	bool gotShot, lastGotShot, gotShield, lastGotShield;
	float runningTime;
	int chapterID;
};

#endif // __GAME_SCENE_H__
