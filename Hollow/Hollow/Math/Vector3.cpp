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
}