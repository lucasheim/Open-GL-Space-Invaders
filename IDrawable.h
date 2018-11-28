#pragma once
#include "Buffer.h"
#include "GameConfig.h"
#include "Sprite.h"

class IDrawable
{
public:
	uint32_t color = WHITE_UINT32;
	int x, y, width, height;
	virtual ~IDrawable() {};
	virtual void draw(Buffer* buffer) = 0;
};

