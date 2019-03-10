#pragma once

class Math
{
public:
	static constexpr float PI = 3.1459;

	static float toRadians(float val)
	{
		return val * 180 / PI;
	}
};