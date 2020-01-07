#include "Vector2.h"

namespace Hollow {
	const Vector2 Vector2::operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	const Vector2 Vector2::operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 Vector2::normalize(const Vector2& vec)
	{
		float length = sqrt(vec.x * vec.x + vec.y * vec.y);
		return Vector2(vec.x / length, vec.y / length);
	}
}

