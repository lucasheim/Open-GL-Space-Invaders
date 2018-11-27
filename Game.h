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
	size_t width, height;
	size_t numAliens;
	size_t numBullets;
	Alien** aliens;
	Buffer* buffer;
	Player* player;
	Bullet** bullets;
	int score;

	bool spriteOverlapCheck(IDrawable* sprite1, IDrawable* sprite2);
	void createAlienMatrix();

public:
	Game(Buffer* buffer);

	void setup();
	void paintBackground(uint32_t color);
	void updateAliens();
	void drawBullets();
	void drawPlayer();
	void updateDeathCounters();
	void updateBullets();
	void updatePlayer(int movementDirection);
	void createPlayerBullet();
};

