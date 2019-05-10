#pragma once
#include <math.h>

class Math
{
public:
	static constexpr double PI = 3.141592653589793238463;
	static constexpr float  PI_F = 3.14159265358979f;
	static constexpr float  HALF_PI = 1.570796326790f;

	static float toRadians(float degrees)
	{
		return degrees * 180 / PI_F;
	}

	static float toDegrees(float rads)
	{
		return rads * PI_F / 180;
	}

	static double cotan(double val) { 
		return(1 / tan(val));
	}
};