#pragma once
#include "Alien.h"
#include "AlienCrab.h"
#include "AlienOctopus.h"
#include "AlienSquid.h"

class AlienFactory
{
public:
	~AlienFactory() {};
	static Alien* createAlien(size_t line, size_t x, size_t y);
};

