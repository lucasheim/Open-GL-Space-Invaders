#include "AlienFactory.h"

Alien * AlienFactory::createAlien(size_t line, size_t x, size_t y) {
	if (line == 4) {
		return new AlienOctopus(x, y);
	}

	if (line == 3) {
		return new AlienSquid(x, y);
	}

	return new AlienCrab(x, y);
}
