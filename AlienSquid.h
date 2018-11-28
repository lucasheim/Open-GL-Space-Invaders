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
	AlienSquid(int x, int y);
	int getScore();
};



