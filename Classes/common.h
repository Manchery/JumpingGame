#ifndef __COMMON_H__
#define __COMMON_H__

#include "cocos2d.h"
USING_NS_CC;

#define EFFECT(__FILE__) SimpleAudioEngine::getInstance()->playEffect("sounds/"##__FILE__##, false, 1.0f, 1.0f, 1.0f)
#define BGM(__FILE__) SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/loops/"##__FILE__##, true)
#define STOPBGM SimpleAudioEngine::getInstance()->stopBackgroundMusic()
#define PAUSEBGM SimpleAudioEngine::getInstance()->pauseBackgroundMusic()
#define RESUMEBGM  SimpleAudioEngine::getInstance()->resumeBackgroundMusic()

#define HELLOBGM "BGM_SCENARIO_SAD.mp3"
#define ENTRYBGM "spacewind.mp3"
#define CHAPTER0INTROBGM "spacewind.mp3"
#define CHAPTER0BGM "titletheme.wav"
#define CHAPTER1BGM "BGM_OPENING MOVIE.mp3"
#define CHAPTER2BGM "BGM_SCENARIO_SCENE.mp3"
#define CHAPTER3BGM "BGM_SCENARIO_CALM.mp3"
#define CHAPTER4BGM "BGM_BATTLE_LAST DRAGON.mp3"
#define CHAPTER5BGM "bgm_raidboss.mp3"
#define CHAPTER6BGM "BGM_SCENARIO_HOPE.mp3" 

#define INFTIME (1<<30)

#define PHYSICS_DRAW_DEBUG false
#define AUDIO_PLAY true

#define HERO_T 1
#define ENEMY_T 2
#define FOLLOW_ENEMY_T 3
#define BOSS_T 4

#define BULLET_T 10
#define COIN_T 11
#define EXIT_T 12
#define GAME_KEY_T 13

#define BORDER_T 100
#define LAND_T 101
#define SOFT_LAND_T 102
#define SLIDING_LAND_T 103
#define DROP_LAND_T 104
#define SWING_LAND_T 105

#define TRAP_T 200
#define SLIDING_TRAP_T 201
#define SWING_TRAP_T 202
#define DEAD_TRAP_T 203

#define DOOR_T 300
#define DOOR_KEY_T 301

#define WATER_T 400

#define HERO_M 0x01
#define LAND_M 0x02
#define ENEMY_M 0x04
#define BULLET_M 0x08
#define DOOR_M 0x10
#define DOOR_KEY_M 0x20
#define FOLLOW_ENEMY_M 0x40
#define COIN_M 0x80
#define EXIT_M 0x100
#define GAME_KEY_M 0x200
#define WATER_M 0x400
#define BOSS_M 0x800

const int chapterCoinTotal[] = { 20,30,30,30,30,30,30};

float getPhysicsMinX(Sprite *sprite);
float getPhysicsMinY(Sprite *sprite);
float getPhysicsMaxX(Sprite *sprite);
float getPhysicsMaxY(Sprite *sprite);

bool isLand(const cocos2d::Node *node);
bool isTrap(const cocos2d::Node *node);
bool isHero(const cocos2d::Node *node);

void problemLoading(const char* filename);
void logUserDefault();

#define DEFAULT_VOLUMN 100
#define DEFAULT_RESOLUTION "fullscreen"
void setOptionDefault();
void setGameDefault();

#endif // __COMMON_H__