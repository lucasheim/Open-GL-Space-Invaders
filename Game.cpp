#include "Game.h"
#include "AlienCrab.h"

Game::Game(size_t width, size_t height) {
	this->width = width;
	this->height = height;
	this->numAliens = NUM_ALIENS;
	this->numBullets = 0;
	this->aliens = new Alien*[this->numAliens];
	this->player.x = 107;
	this->player.y = 32;
	this->player.life = PLAYER_LIFES;
}

void Game::createAlienMatrix()
{
	for (int y = 0; y < 5; ++y) {
		for (int x = 0; x < 11; ++x) {
			this->aliens[y * 11 + x] = new AlienCrab(16 * x + 20, 17 * y + 128);
		}
	}
}
