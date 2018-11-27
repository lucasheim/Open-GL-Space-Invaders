#pragma once
#include "Buffer.h"
#include "GameConfig.h"
class IDrawable
{
public:
	uint32_t color = WHITE_UINT32;
	virtual ~IDrawable() {};
	virtual void draw(Buffer* buffer) = 0;
};

