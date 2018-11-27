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
			this->aliens[y * 11 + x] = new AlienCrab(16 * x + 20, 17 * y + 128);
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

void Game::decreaseDeathCounters() {
	for (int i = 0; i < this->numAliens; i++) {
		Alien* alien = this->aliens[i];
		alien->updateDeathCounter();
	}
}

void Game::updateBullets() {
	for (int i = 0; i < this->numBullets;) {
		// create method that moves the bullet and returns false if it hits a wall (should this be a BULLET responsability?)
		this->bullets[i]->y += this->bullets[i]->dir;
		// Border Check
		if (this->bullets[i]->y >= this->height || this->bullets[i]->y < this->bullets[i]->getSprite()->height) {
			this->bullets[i] = this->bullets[this->numBullets - 1];
			--this->numBullets;
			continue;
		}

		for (int j = 0; j < this->numAliens; j++) {
			Alien* alien = this->aliens[j];
			if (alien->shot) continue;
			SpriteAnimation& animation = alien->animation;
			size_t currentFrame = animation.time / animation.frameDuration;
			Sprite& alienSprite = *animation.frames[currentFrame];
			bool overlap = this->spriteOverlapCheck(
				*this->bullets[i]->getSprite(),
				this->bullets[i]->x,
				this->bullets[i]->y,
				alienSprite,
				alien->x,
				alien->y
			);

			if (overlap) {
				// this.score += 10 * (4);
				alien->shot = true;
				alien->x -= (alien->getDeathSprite()->width - alienSprite.width) / 2;
				this->bullets[i] = this->bullets[this->numBullets - 1];
				--this->numBullets;
				continue;
			}
		}
		i++;
	}
}

void Game::updatePlayer(int movementDirection) {
	// TODO move this to player class
	if (movementDirection != 0) {
		if (this->player->x + this->player->getSprite()->width + movementDirection >= this->width) {
			this->player->x = this->width - this->player->getSprite()->width;
		}
		else if ((int)this->player->x + movementDirection <= 0) {
			this->player->x = 0;
		}
		else {
			this->player->x += movementDirection;
		}
	}
}

void Game::createPlayerBullet() {
	if (this->numBullets < GAME_MAX_BULLETS) {
		int bulletX = this->player->x + this->player->getSprite()->width / 2;
		int bulletY = this->player->y + this->player->getSprite()->height;
		int direction = 2;
		this->bullets[this->numBullets++] = new Bullet(bulletX, bulletY, direction);
	}
}

bool Game::spriteOverlapCheck(const Sprite& sp_a, size_t x_a, size_t y_a, const Sprite& sp_b, size_t x_b, size_t y_b) {
	if (x_a < x_b + sp_b.width && x_a + sp_a.width > x_b &&
		y_a < y_b + sp_b.height && y_a + sp_a.height > y_b) {
		return true;
	}
	return false;
}