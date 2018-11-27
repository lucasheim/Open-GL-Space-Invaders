#pragma once
#include <stdint.h>
#include "Sprite.h"
#include "Alien.h"
#include "Buffer.h"
#include "Formatter.h"
class AlienSquid : public Alien
{
public:
	Sprite* sprites[];
	AlienSquid(size_t x, size_t y);
	int getScore();
};



