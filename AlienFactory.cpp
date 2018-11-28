#include "AlienFactory.h"

Alien * AlienFactory::createAlien(int line, int x, int y) {
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
