#pragma once
#include "Sprite.h"
class Player
{
public:
	size_t x, y;
	size_t life;
	Player(size_t x, size_t y, size_t life);
	Sprite* getSprite();
};

