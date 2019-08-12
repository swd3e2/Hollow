#pragma once

#ifndef HW_MATRIX_4_H
#define HW_MATRIX_4_H

#include <cmath>
#include "Math.h"
#include "Vector4.h"

namespace Hollow {
	class Matrix4
	{
	public:
		union
		{
			float m[16];
			float md[4][4];

			struct
			{
				float m11;
				float m12;
				float m13;
				float m14;
				float m21;
				float m22;
				float m23;
				float m24;
				float m31;
				float m32;
				float m33;
				float m34;
				float m41;
				float m42;
				float m43;
				float m44;
			} mm;

			struct
			{
				Vector4 v1;
				Vector4 v2;
				Vector4 v3;
				Vector4 v4;
			} v;
		};
	public:
		Matrix4();
		Matrix4(const Matrix4& other);
		Matrix4(Matrix4&& other);
		Matrix4(const float* other, int size);
		Matrix4(const double* other, int size);

		Matrix4& operator=(const Matrix4& other);
		Matrix4& operator=(Matrix4&& other);

		static Matrix4 identity();

		Matrix4& transpose();
		static Matrix4 transpose(const Matrix4& matrix);

		static Matrix4 translation(float x, float y, float z);
		static Matrix4 translation(const Vector4& vec);

		static Matrix4 scaling(float x, float y, float z);
		static Matrix4 scaling(const Vector4& vec);

		static Matrix4 rotation(const Vector4& vec);
		static Matrix4 rotation(float x, float y, float z);

		static Matrix4 rotationX(float x);
		static Matrix4 rotationY(float y);
		static Matrix4 rotationZ(float z);

		Matrix4 operator*(const Matrix4& other) const;

		static Matrix4 projection(float fov, float aspect, float n, float f);
		static Matrix4 orthographic(float right, float left, float top, float bottom, float near, float far);
		static Matrix4 lookAt(const Vector4& eyePosition, const Vector4& eyeDirection, const Vector4& upVector);

		void setTranslation(const Vector4& vecPos);
		Vector4 getTranslation() const;

		static Matrix4 inverse(const Matrix4& mat);
	};

	Vector4 operator*(const Vector4& vec, const Matrix4& mat);
	Vector4 operator*(const Matrix4& mat, const Vector4& vec);
	Vector3 operator*(const Vector3& vec, const Matrix4& mat);
	Vector3 operator*(const Matrix4& mat, const Vector3& vec);
}

#endif