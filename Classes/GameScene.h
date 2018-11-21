#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Hero.h"
#include "Enemy.h"
#include "FollowEnemy.h"
#include "common.h"

USING_NS_CC;

class GameScene : public cocos2d::Scene
{
public:
	virtual void onEnterTransitionDidFinish();
	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void initMap();
	virtual void initListener();

	bool onContactBegin(cocos2d::PhysicsContact &contact);
	bool onContactPreSolve(PhysicsContact& contact, PhysicsContactPreSolve& solve);
	bool onContactPostSolve(PhysicsContact& contact, const PhysicsContactPostSolve& solve);
	bool onContactEnd(cocos2d::PhysicsContact &contact);
	bool onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	bool onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void heroUpdate(float dt);
	void mapUpdate(float dt);
	void regenerateUpdate(float dt);

	void addRenerate(Node * node);

	Vec2 getHeroGlobalPosition();

	void gamePause();
	void heroDie();
	void heroJump();

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
	bool heroDied,heroJumped;
};

#endif // __GAME_SCENE_H__
