#pragma once
#include "Alien.h"
#include "GameConfig.h"
#include "AlienCrab.h"
#include "Buffer.h"
#include "Formatter.h"
#include "Bullet.h"
#include "Player.h"
#include "AlienFactory.h"
#include <stdlib.h> 
#include <cstdio>

class Game
{
	int width, height;
	int numAliens;
	int numBullets;
	int killedAliens;
	Alien** aliens;
	Buffer* buffer;
	Player* player;
	Bullet** bullets;
	int score;

	bool spriteOverlapCheck(IDrawable* sprite1, IDrawable* sprite2);
	void createAlienMatrix();
	void newAlienWave();
	void restartGame();

public:
	Game(Buffer* buffer);

	void setup();
	void paintBackground(uint32_t color);
	void updateAliens();
	void drawBullets();
	void drawPlayer();
	void drawScore();
	void updateDeathCounters();
	void updateBullets();
	void updatePlayer(int movementDirection);
	void createPlayerBullet();
	void enemyFire();
};

