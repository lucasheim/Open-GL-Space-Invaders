#pragma once
#include "Alien.h"
#include "GameConfig.h"

class Game
{
public:
	size_t width, height;
	size_t numAliens;
	size_t numBullets;
	Alien** aliens;
	Player player;
	Bullet bullets[GAME_MAX_BULLETS];

	Game(size_t width, size_t height);

	void createAlienMatrix();
};

