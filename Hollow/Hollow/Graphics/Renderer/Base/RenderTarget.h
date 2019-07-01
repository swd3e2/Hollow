#pragma once

#ifndef HW_RENDER_TARGET_H
#define HW_RENDER_TARGET_H

#include "Hollow/Math/Vector4.h"

namespace Hollow {
	enum RENDER_TARGET_TEXTURE_FORMAT
	{
		R8G8B8A8 = 0,
		R32G32B32A32 = 1
	};

	struct RENDER_TARGET_DESC
	{
		int count = 1;
		RENDER_TARGET_TEXTURE_FORMAT textureFormat;

		struct {

		} colorBufferDesc;

		struct {

		} depthStencilBufferDesc;
	};

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
		int count;
	public:
		RenderTarget(int width, int height, int count) :
			width(width), height(height), count(count)
		{}

		int getWidth() const { return width; }
		int getHeight() const { return height; }

		virtual Vector4 readPixel(int x, int y) = 0;
	};
}

#endif