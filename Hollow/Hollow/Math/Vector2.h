#pragma once

#ifndef HW_VECTOR_2_H
#define HW_VECTOR_2_H

#include <math.h>

namespace Hollow {
	class Vector2
	{
	public:
		float x;
		float y;
	public:
		Vector2() :
			x(0.0f), y(0.0f)
		{}

		Vector2(float x, float y) :
			x(x), y(y)
		{}

		const Vector2 operator-(const Vector2& other) const;
		const Vector2 operator+(const Vector2& other) const;

		static Vector2 normalize(const Vector2& vec);
	};
}

#endif