#include "Game.h"

Game::Game(Buffer* buffer) {
	this->width = buffer->width;
	this->height = buffer->height;
	this->numAliens = NUM_ALIENS;
	this->numBullets = 0;
	this->aliens = new Alien*[this->numAliens];
	this->buffer = buffer;
	this->player = new Player(107, 32, PLAYER_LIFES);
	this->bullets = new Bullet*[GAME_MAX_BULLETS];
}

void Game::createAlienMatrix() {
	for (int y = 0; y < 5; ++y) {
		for (int x = 0; x < 11; ++x) {
			this->aliens[y * 11 + x] = AlienFactory::createAlien(y, 16 * x + 20, 17 * y + 128);
		}
	}
}

void Game::setup() {
	this->createAlienMatrix();
}

void Game::paintBackground(uint32_t color) {
	this->buffer->colorClear(color);
}

void Game::updateAliens() {
	for (int i = 0; i < this->numAliens; i++) {
		Alien* alien = this->aliens[i];
		if (!alien->isDead()) {
			alien->draw(this->buffer);
		}
	}
}

void Game::drawBullets() {
	for (int i = 0; i < this->numBullets; i++) {
		Bullet* bullet = this->bullets[i];
		bullet->draw(this->buffer);
	}
}

void Game::drawPlayer() {
	this->player->draw(this->buffer);
}

void Game::updateDeathCounters() {
	for (int i = 0; i < this->numAliens; i++) {
		Alien* alien = this->aliens[i];
		alien->updateDeathCounter();
	}
}

void Game::updateBullets() {
	for (int i = 0; i < this->numBullets;) {
		Bullet* bullet = this->bullets[i];
		bullet->move();
		// Border Check
		if (bullet->y >= this->height || //passed top border
			bullet->y <  bullet->height) //passed bottom border
		{
			this->bullets[i] = this->bullets[--this->numBullets];
			continue;
		}

		for (int j = 0; j < this->numAliens; j++) {
			Alien* alien = this->aliens[j];
			if (alien->hasBeenShot()) continue;
			bool overlap = this->spriteOverlapCheck(bullet, alien);

			if (overlap) {
				this->score += alien->getScore();
				alien->shot = true;
				alien->x -= (alien->getDeathSprite()->width - alien->width) / 2;
				this->bullets[i] = this->bullets[this->numBullets - 1];
				--this->numBullets;
				continue;
			}
		}
		i++;
	}
}

void Game::updatePlayer(int movementDirection) {
	this->player->move(movementDirection, this->width);
}

void Game::createPlayerBullet() {
	if (this->numBullets < GAME_MAX_BULLETS) {
		int bulletX = this->player->x + this->player->width / 2;
		int bulletY = this->player->y + this->player->height;
		int direction = PLAYER_BULLET_DIRECTION;
		this->bullets[this->numBullets] = new Bullet(bulletX, bulletY, direction);
		this->numBullets++;
	}
}

bool Game::spriteOverlapCheck(IDrawable* sprite1, IDrawable* sprite2) {
	size_t x1 = sprite1->x;
	size_t x2 = sprite2->x;
	size_t y1 = sprite1->y;
	size_t y2 = sprite2->y;
	size_t w1 = sprite1->width;
	size_t w2 = sprite2->width;
	size_t h1 = sprite1->height;
	size_t h2 = sprite2->height;
	if (x1 < (x2 + w2) && (x1 + w1) > x2 &&
		y1 < (y2 + h2) && (y1 + h1) > y2) {
		return true;
	}
	return false;
}