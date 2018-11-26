#pragma once
#include "Buffer.h"
class IDrawable
{
public:
	virtual ~IDrawable() {};
	virtual void draw(Buffer* buffer) = 0;
};

