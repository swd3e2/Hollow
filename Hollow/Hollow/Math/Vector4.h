#pragma once

#ifndef HW_VECTOR_4_H
#define HW_VECTOR_4_H

#include <utility>
#include "Vector3.h"
#include "immintrin.h"

namespace Hollow {
	class alignas(sizeof(__m128)) Vector4
	{
	public:
		float x;
		float y;
		float z;
		float w;
	public:
		Vector4();
		Vector4(float x, float y, float z, float w);
		Vector4(const Vector3& other);

		Vector4(const Vector4&) = default;
		Vector4(Vector4&&) = default;

		Vector4& operator=(const Vector4& other);
		Vector4& operator=(Vector4&& other);

		Vector4 operator*(const float val);
		Vector4 operator*(const double val);
		float operator*(const Vector4& other);

		Vector4 cross(const Vector4& other);
		static Vector4 cross(const Vector4& left, const Vector4& right);

		static float dot(const Vector4& left, const Vector4& right);

		Vector4 operator+(const Vector4& other);
		void operator+=(const Vector4& other);

		Vector4 operator-(const Vector4& other) const;
		void operator-=(const Vector4& other);
		
		operator __m128*() const
		{
			return (__m128*)this;
		}
		
		Vector4& operator=(const __m128& other)
		{
			x = other.m128_f32[0];
			y = other.m128_f32[1];
			z = other.m128_f32[2];
			w = other.m128_f32[3];

			return *this;
		}

		static Vector4 normalize(const Vector4& vector);
		static Vector4 negate(const Vector4& vector);
	};
}

#endif