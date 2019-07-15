#pragma once

#ifndef HW_RENDER_TARGET_H
#define HW_RENDER_TARGET_H

#include "Hollow/Math/Vector4.h"

namespace Hollow {
	enum RENDER_TARGET_TEXTURE_FORMAT
	{
		R8G8B8A8,
		R32G32B32A32
	};

	struct RENDER_TARGET_DESC
	{
		int width;
		int height;
		int count = 1;
		RENDER_TARGET_TEXTURE_FORMAT textureFormat;
	};

	class RenderTarget
	{
	private:
		int width;
		int height;
	public:
		int count;
	public:
		RenderTarget(int width, int height, int count) :
			width(width), height(height), count(count)
		{}

		static RenderTarget* create(RENDER_TARGET_DESC desc);

		int getWidth() const { return width; }
		int getHeight() const { return height; }
		virtual Vector4 readPixel(int x, int y) = 0;
	};
}

#endif