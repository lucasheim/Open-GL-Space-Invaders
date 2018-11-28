#pragma once
#include "Alien.h"
#include "AlienCrab.h"
#include "AlienOctopus.h"
#include "AlienSquid.h"

class AlienFactory {
public:
	~AlienFactory() {};
	static Alien* createAlien(int line, int x, int y);
};

