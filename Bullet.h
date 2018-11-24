#pragma once
#include "Sprite.h"
class Bullet
{
public:
	size_t x, y;
	int dir;
	Bullet(size_t x, size_t y, int dir);
	Sprite* getSprite();
};

