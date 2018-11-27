#include "AlienFactory.h"

Alien * AlienFactory::createAlien(size_t line, size_t x, size_t y) {
	switch (line) {
		case 4:
			return new AlienOctopus(x, y);
		case 3:
			return new AlienSquid(x, y);
		default:
			return new AlienCrab(x, y);
			break;
	}
}
