#pragma once

#ifndef HW_MATRIX_4_H
#define HW_MATRIX_4_H

#include <cmath>
#include "Math.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "immintrin.h"

namespace Hollow {
	class alignas (sizeof(__m256)) Matrix4
	{
	public:
		Vector4 r[4];
	public:
		Matrix4() = default;
		Matrix4(float _a1, float _a2, float _a3, float _a4,
				float _b1, float _b2, float _b3, float _b4,
				float _c1, float _c2, float _c3, float _c4,
				float _d1, float _d2, float _d3, float _d4);
		Matrix4(const Matrix4& other);
		Matrix4(const float* other, int size);

		Matrix4& operator=(const Matrix4& other);

		static Matrix4 identity();

		Matrix4& transpose();
		static Matrix4 transpose(const Matrix4& matrix);

		static Matrix4 translation(float x, float y, float z);
		static Matrix4 translation(const Vector4& vec);

		static Matrix4 scaling(float x, float y, float z);
		static Matrix4 scaling(const Vector4& vec);

		static Matrix4 rotation(const Vector4& vec);
		static Matrix4 rotation(float x, float y, float z);
		static Matrix4 rotation(const Quaternion& quat);

		static Matrix4 rotationX(float x);
		static Matrix4 rotationY(float y);
		static Matrix4 rotationZ(float z);

		Matrix4 operator*(const Matrix4& other) const;

		static Matrix4 projection(float fov, float aspect, float n, float f);
		static Matrix4 orthographic(float right, float left, float top, float bottom, float near, float far);
		static Matrix4 lookAt(const Vector4& eyePosition, const Vector4& eyeDirection, const Vector4& upVector);

		static Matrix4 inverse(const Matrix4& mat);
	private:
		inline void swap(float& first, float& second)
		{
			float temp = first;
			first = second;
			second = temp;
		}
	};

	Vector4 operator*(const Vector4& vec, const Matrix4& mat);
	Vector4 operator*(const Matrix4& mat, const Vector4& vec);
	Vector3 operator*(const Vector3& vec, const Matrix4& mat);
	Vector3 operator*(const Matrix4& mat, const Vector3& vec);
}

#endif