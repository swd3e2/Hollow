#pragma once

#ifndef HW_RENDER_TARGET_H
#define HW_RENDER_TARGET_H

#include "Hollow/Math/Vector4.h"

namespace Hollow {
	enum RenderTargetFlags {
		NONE = 0,
		ACCESS_BY_CPU = 1
	};

	class RenderTarget
	{
	private:
		int width;
		int height;
	public:
		RenderTarget(int width, int height) :
			width(width), height(height)
		{}

		int getWidth() const { return width; }
		int getHeight() const { return height; }

		virtual Vector4 readPixel(int x, int y) = 0;
	};
}

#endif