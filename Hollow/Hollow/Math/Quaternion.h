#pragma once
#include "Math.h"
#include <cmath>

class Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;
public:
	Quaternion(float x, float y, float z, float w)
	{
		float rads = Math::toRadians(w);

		w = cosf(rads/2);

		float sin = sinf(rads / 2);
		x = x * sin;
		y = y * sin;
		z = z * sin;
	}
};