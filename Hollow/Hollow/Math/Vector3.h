#pragma once

#ifndef HW_VECTOR_3_H
#define HW_VECTOR_3_H
#include <utility>

namespace Hollow {
	class Vector3
	{
	public:
		float x;
		float y;
		float z;
	public:
		Vector3();
		Vector3(float x, float y, float z);

		Vector3(const Vector3& vec);
		Vector3(Vector3&& vec);

		Vector3& operator=(const Vector3& vec);
		Vector3& operator=(Vector3&& vec);

		Vector3 operator-(const Vector3& other) const;
		Vector3 operator/(const float divisor) const;
		Vector3& invert();

		Vector3& operator*(const float val);
		static Vector3 cross(const Vector3& left, const Vector3& right);
		static Vector3 normalize(const Vector3& vector);
		static Vector3 interpolate(const Vector3& left, const Vector3& right, const float factor);
		static float length(const Vector3& vec);
		static float dot(const Vector3& left, const Vector3& right);

		Vector3& operator+=(const Vector3& other);
		Vector3 operator+(const Vector3& other) const;
	};
}

#endif