#pragma once
#include <stdint.h>
#include "Sprite.h"
#include "Alien.h"
#include "Buffer.h"
#include "Formatter.h"
class AlienCrab : public Alien
{
public:
	Sprite* sprites[];
	AlienCrab(size_t x, size_t y);
	void draw(Buffer* buffer);
};

