#pragma once

#define GAME_MAX_BULLETS 128
#define NUM_ALIENS 55
#define PLAYER_LIFES 3

struct Player
{
	size_t x, y;
	size_t life;
};

struct Bullet
{
	size_t x, y;
	int dir;
};