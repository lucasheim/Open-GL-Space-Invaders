#pragma once
#include <stdint.h>
#include "Sprite.h"
#include "Alien.h"
class AlienSquid : public Alien
{
	size_t width, height;

public:
	Sprite* sprites[];
	AlienSquid(size_t x, size_t y);
};

