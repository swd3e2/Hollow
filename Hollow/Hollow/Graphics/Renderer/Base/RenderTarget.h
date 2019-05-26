#pragma once

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