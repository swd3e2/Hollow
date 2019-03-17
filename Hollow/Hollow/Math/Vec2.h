#pragma once
#include <utility>

class Vec2
{
public:
	float x;
	float y;
public:
	Vec2(float x, float y) : x(x), y(y) {}

	Vec2(Vec2& second) 
	{
		x = second.x;
		y = second.y;
	}

	Vec2&& operator-(const Vec2 second)
	{
		return std::move(Vec2(x - second.x, y - second.y));
	}

	void operator+(const Vec2 second) 
	{
		x += second.x;
		y += second.y;
	}

	void operator/(float val)
	{
		x /= val;
		y /= val;
	}

	void operator*(float val)
	{
		x *= val;
		y *= val;
	}
};