#include "Vector3.h"

namespace Hollow {
	Vector3::Vector3() :
		x(0), y(0), z(0)
	{}

	Vector3::Vector3(float x, float y, float z) :
		x(x), y(y), z(z)
	{}

	Vector3::Vector3(const Vector3& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}

	Vector3::Vector3(Vector3&& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}

	Vector3& Vector3::operator=(const Vector3& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;

		return *this;
	}

	Vector3& Vector3::operator=(Vector3&& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;

		return *this;
	}

	Vector3 Vector3::operator-(const Vector3& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	Vector3 Vector3::operator/(const float divisor) const
	{
		return Vector3(x / divisor, y / divisor, z / divisor);
	}

	Vector3& Vector3::operator*(const float val)
	{
		x *= val;
		y *= val;
		z *= val;

		return *this;
	}

	Vector3 Vector3::cross(const Vector3& left, const Vector3& right)
	{
		float x = left.y * right.z - left.z * right.y;
		float y = left.z * right.x - left.x * right.z;
		float z = left.x * right.y - left.y * right.x;

		return Vector3(x, y, z);
	}

	Vector3 Vector3::normalize(const Vector3& vector)
	{
		float length = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
		return Vector3(vector.x / length, vector.y / length, vector.z / length);
	}

	float Vector3::dot(const Vector3& left, const Vector3& right)
	{
		return left.x * right.x + left.y * right.y + left.z * right.z;
	}

	Vector3& Vector3::invert()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	Vector3 Vector3::operator+(const Vector3& vec) const
	{
		return Hollow::Vector3(x + vec.x, y + vec.y, z + vec.z);
	}

	Vector3& Vector3::operator+=(const Vector3& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;

		return *this;
	}

	float Vector3::length(const Vector3& vec)
	{
		return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	}
}