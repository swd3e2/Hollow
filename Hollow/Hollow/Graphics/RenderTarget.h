#pragma once

#ifndef HW_RENDER_TARGET_H
#define HW_RENDER_TARGET_H

#include "Hollow/Platform.h"
#include "Hollow/Math/Vector4.h"
#include "CommonTypes.h"

namespace Hollow {
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
		int count;
	public:
		RenderTarget(int width, int height, int count) :
			width(width), height(height), count(count)
		{}

		virtual ~RenderTarget() {}

		static s_ptr<RenderTarget> create(RENDER_TARGET_DESC desc);

		inline int getWidth() const { return width; }
		inline int getHeight() const { return height; }
		inline int getCount() const { return count; }
		virtual Vector4 readPixel(int x, int y) = 0;
	};
}

#endif