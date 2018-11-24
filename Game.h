#pragma once
#include "Alien.h"
#include "GameConfig.h"
#include "AlienCrab.h"
#include "Buffer.h"
#include "Formatter.h"
#include "Bullet.h"
#include "Player.h"

class Game
{
	bool spriteOverlapCheck(const Sprite& sp_a, size_t x_a, size_t y_a, const Sprite& sp_b, size_t x_b, size_t y_b);
public:
	size_t width, height;
	size_t numAliens;
	size_t numBullets;
	Alien** aliens;
	Buffer* buffer;
	Player* player;
	Bullet** bullets;

	Game(Buffer* buffer);

	void createAlienMatrix();
	void updateAliens();
	void drawBullets();
	void drawPlayer();
	void decreaseDeathCounters();
	void updateBullets();
};

