#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Hero.h"
#include "Enemy.h"

USING_NS_CC;

class GameScene : public cocos2d::Scene
{
public:

	static cocos2d::Scene* createScene();
	virtual bool init();
	virtual void initMap();
	virtual void initListener();

	void menuExitCallback(cocos2d::Ref* pSender);
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

	void screenShot();
	void gamePause();
	void heroDie();
	void heroJump();

	CREATE_FUNC(GameScene);

protected:
	Layer *frontGroundLayer, *backGroundLayer;
	std::vector<Node*> destroyedList;
	std::vector<std::pair<Node*, float> > regenList;
	Size mapSize;
	Hero *hero;
	bool upKeyDown, leftKeyDown, rightKeyDown, downKeyDown;
	cocos2d::EventKeyboard::KeyCode lastKey;
	Vec2 revivePoint;
	bool heroDied,heroJumped;
};

#endif // __GAME_SCENE_H__
