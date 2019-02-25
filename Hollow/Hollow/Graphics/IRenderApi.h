#pragma once
#include "IRenderable.h"

class IRenderApi
{
public:
	virtual void Draw(IRenderable& renderable) = 0;
};