#pragma once

#ifndef HW_VECTOR_4_H
#define HW_VECTOR_4_H

#include <utility>
#include "Vector3.h"
#include "Hollow/Platform.h"

class Matrix4;

class HOLLOW_API Vector4
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

	Vector4 operator*(float val);
	float operator*(const Vector4& other);

	Vector4 Cross(const Vector4& other);
	static Vector4 Cross(const Vector4& left, const Vector4& right);

	static float Dot(const Vector4& left, const Vector4& right);

	Vector4 operator+(const Vector4& other);
	void operator+=(const Vector4& other);

	Vector4 operator-(const Vector4& other) const;
	void operator-=(const Vector4& other);
	
	static Vector4 Normalize(const Vector4& vector);
	static Vector4 Negative(const Vector4& vector);
};

#endif