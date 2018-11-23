#ifndef __COMMON_H__
#define __COMMON_H__

#include "cocos2d.h"

#define PHYSICS_DRAW_DEBUG true
#define AUDIO_PLAY false

#define HERO_T 1
#define ENEMY_T 2
#define FOLLOW_ENEMY_T 3

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

#define DOOR_T 300
#define DOOR_KEY_T 301

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

const int chapterCoinTotal[] = { 12};

bool isLand(const cocos2d::Node *node);
bool isHero(const cocos2d::Node *node);
bool touchUpSurface(const cocos2d::Node *nodeA, const cocos2d::Node *nodeB);

void problemLoading(const char* filename);

#endif // __COMMON_H__