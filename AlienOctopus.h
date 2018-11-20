#pragma once
#include <stdint.h>
#include "Sprite.h"
#include "Alien.h"
class AlienOctopus : public Alien
{
public:
	Sprite* sprites[];
	AlienOctopus(size_t x, size_t y);
};

