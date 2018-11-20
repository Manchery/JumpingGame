#pragma once

#define PHYSICS_DRAW_DEBUG true
#define AUDIO_PLAY false

#define HERO_T 1
#define ENEMY_T 2

#define BULLET_T 10

#define BORDER_T 100
#define LAND_T 101
#define SOFT_LAND_T 102
#define SLIDING_LAND_T 103
#define DROP_LAND_T 104
#define TRAP_T 105

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}