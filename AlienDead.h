#pragma once
#include <stdint.h>
#include "Sprite.h"
#include "Alien.h"
class AlienDead : public Alien
{
public:
	Sprite* sprites[];
	AlienDead(size_t x, size_t y);
};

