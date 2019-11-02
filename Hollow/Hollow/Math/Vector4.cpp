#include "Vector4.h"

namespace Hollow {
	Vector4::Vector4() :
		x(0), y(0), z(0), w(0.0f)
	{}

	Vector4::Vector4(float x, float y, float z, float w) :
		x(x), y(y), z(z), w(w)
	{}

	Vector4::Vector4(const Vector3& other) :
		x(other.x), y(other.y), z(other.z), w(1.0f)
	{}

	Vector4& Vector4::operator=(const Vector4& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;

		return *this;
	}

	Vector4& Vector4::operator=(Vector4&& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;

		return *this;
	}

	Vector4 Vector4::operator*(const float val)
	{
		return Vector4(x * val, y * val, z * val, w);
	}

	Vector4 Vector4::operator*(const double val)
	{
		return Vector4(x * val, y * val, z * val, w);
	}

	float Vector4::operator*(const Vector4& other)
	{
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	Vector4 Vector4::cross(const Vector4& other)
	{
		float x = y * other.z - z * other.y;
		float y = z * other.x - x * other.z;
		float z = x * other.y - y * other.x;

		return Vector4(x, y, z, 0.0f);
	}

	Vector4 Vector4::cross(const Vector4& left, const Vector4& right)
	{
		float x = left.y * right.z - left.z * right.y;
		float y = left.z * right.x - left.x * right.z;
		float z = left.x * right.y - left.y * right.x;

		return Vector4(x, y, z, 1.0f);
	}

	float Vector4::dot(const Vector4& left, const Vector4& right)
	{
		return left.x * right.x + left.y * right.y + left.z * right.z;
	}

	Vector4 Vector4::operator+(const Vector4& other)
	{
		return Vector4(x + other.x, y + other.y, z + other.z, 1.0f);
	}

	void Vector4::operator+=(const Vector4& other)
	{
		x = x + other.x;
		y = y + other.y;
		z = z + other.z;
	}

	Vector4 Vector4::operator-(const Vector4& other) const
	{
		return Vector4(x - other.x, y - other.y, z - other.z, 1.0f);
	}

	void Vector4::operator-=(const Vector4& other)
	{
		x = x - other.x;
		y = y - other.y;
		z = z - other.z;
	}

	Vector4 Vector4::normalize(const Vector4& vector)
	{
		float length = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
		return Vector4(vector.x / length, vector.y / length, vector.z / length, vector.w);
	}

	Vector4 Vector4::negate(const Vector4& vector)
	{
		return Vector4(-vector.x, -vector.y, -vector.z, -vector.w);
	}
}