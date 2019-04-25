#pragma once

#ifndef HW_MATRIX_4_H
#define HW_MATRIX_4_H

#include <cmath>
#include "Math.h"
#include "Vector4.h"
#include "Hollow/Platform.h"

class HOLLOW_API Matrix4
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

	Matrix4& operator=(const Matrix4& other);
	Matrix4& operator=(Matrix4&& other);

	static Matrix4 Identity();

	Matrix4& Transpose();
	static Matrix4 Transpose(const Matrix4& matrix);

	static Matrix4 Translation(float x, float y, float z);
	static Matrix4 Translation(const Vector4& vec);

	static Matrix4 Scaling(float x, float y, float z);
	static Matrix4 Scaling(const Vector4& vec);

	static Matrix4 Rotation(const Vector4& vec);
	static Matrix4 Rotation(float x, float y, float z);

	static Matrix4 RotationX(float x);
	static Matrix4 RotationY(float y);
	static Matrix4 RotationZ(float z);

	Matrix4 operator*(const Matrix4& other) const;

	static Matrix4 Projection(float fov, float aspect, float n, float f);
	static Matrix4 LookAt(const Vector4& eyePosition, const Vector4& eyeDirection, const Vector4& upVector);

	void SetTranslation(const Vector4& vecPos);
	Vector4 GetTranslation() const;
};

Vector4 operator*(const Vector4& vec, const Matrix4& mat);

#endif