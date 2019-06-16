#pragma once

#ifndef HW_RENDER_TARGET_H
#define HW_RENDER_TARGET_H

namespace Hollow {
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
	};
}

#endif