#include "Vector4.h"

Vector4::Vector4() :
	x(0), y(0), z(0), w(0)
{}

Vector4::Vector4(float x, float y, float z, float w) :
	x(x), y(y), z(z), w(w)
{}

Vector4::Vector4(const Vector3 & other) :
	x(other.x), y(other.y), z(other.z), w(1.0f)
{}

Vector4 & Vector4::operator=(const Vector4 & other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;

	return *this;
}

Vector4 & Vector4::operator=(Vector4 && other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;

	return *this;
}

Vector4::Vector4(std::initializer_list<float>& l)
{
	auto it = l.begin();
	x = *it++;
	y = *it++;
	z = *it++;
	w = *it;
}

Vector4 Vector4::operator*(float val)
{
	return Vector4(x *val, y * val, z * val, w);
}

float Vector4::operator*(const Vector4 & other)
{
	return x * other.x + y * other.y + z * other.z + w * other.w;
}

Vector4 Vector4::Cross(const Vector4 & other)
{
	float x = y * other.z - z * other.y;
	float y = z * other.x - x * other.z;
	float z = x * other.y - y * other.x;

	return Vector4(x, y, z, 0.0f);
}

Vector4 Vector4::Cross(const Vector4 & left, const Vector4 & right)
{
	float x = left.y * right.z - left.z * right.y;
	float y = left.z * right.x - left.x * right.z;
	float z = left.x * right.y - left.y * right.x;

	return Vector4(x, y, z, 1.0f);
}

Vector4 Vector4::operator+(const Vector4 & other)
{
	return Vector4(x + other.x, y + other.y, z + other.z, 1.0f);
}

void Vector4::operator+=(const Vector4 & other)
{
	x = x + other.x;
	y = y + other.y;
	z = z + other.z;
}

Vector4 Vector4::operator-(const Vector4 & other)
{
	return Vector4(x - other.x, y - other.y, z - other.z, 1.0f);
}

void Vector4::operator-=(const Vector4 & other)
{
	x = x - other.x;
	y = y - other.y;
	z = z - other.z;
}

Vector4 Vector4::Normalize(const Vector4 & vector)
{
	float length = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w);
	return Vector4(vector.x / length, vector.y / length, vector.z / length, vector.w / length);
}

Vector4 Vector4::Negatate(const Vector4 & vector)
{
	return Vector4(-vector.x, -vector.y, -vector.z, -vector.w);
}

Vector4 Vector4::Dot(const Vector4& left, const Vector4& right)
{
	float dot = left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
	return Vector4(dot, dot, dot, dot);
}

Vector4 Vector4::Select(const Vector4 & v1, const Vector4 & v2, const Vector4 & control)
{
	union Kek
	{
		Kek() {}
		Vector4 vec;
		unsigned int uints[4];
	};

	union Kek2
	{
		Kek2() {}
		float fval;
		unsigned int uintval;
	};

	Kek kek1;
	kek1.vec = v1;

	Kek kek2;
	kek2.vec = v2;

	Kek kek3;
	kek3.vec = control;

	Kek2 res[4];
	res[0].uintval = (kek1.uints[0] & ~kek3.uints[0]) | (kek2.uints[0] & kek3.uints[0]);
	res[1].uintval = (kek1.uints[1] & ~kek3.uints[1]) | (kek2.uints[1] & kek3.uints[1]);
	res[2].uintval = (kek1.uints[2] & ~kek3.uints[2]) | (kek2.uints[2] & kek3.uints[2]);
	res[3].uintval = (kek1.uints[3] & ~kek3.uints[3]) | (kek2.uints[3] & kek3.uints[3]);

	Vector4 Result = Vector4(
		res[0].fval,
		res[1].fval,
		res[2].fval,
		res[3].fval
		);
	return Vector4();
}
