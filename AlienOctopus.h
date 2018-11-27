#pragma once
#include <stdint.h>
#include "Sprite.h"
#include "Alien.h"
#include "Buffer.h"
#include "Formatter.h"
class AlienOctopus : public Alien
{
public:
	Sprite* sprites[];
	AlienOctopus(size_t x, size_t y);
	int getScore();
};


