#pragma once

#ifndef HW_QUATERNION_H
#define HW_QUATERNION_H

#include "Math.h"
#include <cmath>
#include "Matrix4.h"

namespace Hollow {
	class Quaternion
	{
	public:
		float x;
		float y;
		float z;
		float w;
	public:
		Quaternion() :
			x(0), y(0), z(0), w(0)
		{}

		Quaternion(float x, float y, float z, float w) :
			x(x), y(y), z(z), w(w)
		{}

		Quaternion(const Quaternion& other) :
			x(other.x), y(other.y), z(other.z), w(other.w)
		{}

		Quaternion(Quaternion&& other) noexcept :
			x(other.x), y(other.y), z(other.z), w(other.w)
		{}

		Quaternion& operator=(const Quaternion& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;

			return *this;
		}

		Quaternion& operator=(Quaternion&& other) noexcept
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;

			return *this;
		}

		static inline void Interpolate(Quaternion& pOut, const Quaternion& pStart, const Quaternion& pEnd, float pFactor)
		{
			// calc cosine theta
			float cosom = pStart.x * pEnd.x + pStart.y * pEnd.y + pStart.z * pEnd.z + pStart.w * pEnd.w;

			// adjust signs (if necessary)
			Quaternion end = pEnd;
			if (cosom < static_cast<float>(0.0))
			{
				cosom = -cosom;
				end.x = -end.x;   // Reverse all signs
				end.y = -end.y;
				end.z = -end.z;
				end.w = -end.w;
			}

			// Calculate coefficients
			float sclp, sclq;
			if ((static_cast<float>(1.0) - cosom) > static_cast<float>(0.0001)) // 0.0001 -> some epsillon
			{
				// Standard case (slerp)
				float omega, sinom;
				omega = std::acos(cosom); // extract theta from dot product's cos theta
				sinom = std::sin(omega);
				sclp = std::sin((static_cast<float>(1.0) - pFactor) * omega) / sinom;
				sclq = std::sin(pFactor * omega) / sinom;
			}
			else
			{
				// Very close, do linear interp (because it's faster)
				sclp = static_cast<float>(1.0) - pFactor;
				sclq = pFactor;
			}

			pOut.x = sclp * pStart.x + sclq * end.x;
			pOut.y = sclp * pStart.y + sclq * end.y;
			pOut.z = sclp * pStart.z + sclq * end.z;
			pOut.w = sclp * pStart.w + sclq * end.w;
		}

		Matrix4 toMatrix4()
		{
			Matrix4 matrix;

			matrix.md[0][0] = 1 - 2 * (y * y) - 2 * (z * z);
			matrix.md[0][1] = 2 * x * y - 2 * z * w;
			matrix.md[0][2] = 2 * x * z + 2 * y * w;

			matrix.md[1][0] = 2 * x * y + 2 * z * w;
			matrix.md[1][1] = 1 - 2 * (x * x) - 2 * (z * z);
			matrix.md[1][2] = 2 * y * z - 2 * x * w;

			matrix.md[2][0] = 2 * x * z - 2 * y * w;
			matrix.md[2][1] = 2 * y * z + 2 * x * w;
			matrix.md[2][2] = 1 - 2 * (x * x) - 2 * (y * y);

			/*matrix.md[0][0] = 1 - 2 * (y * y) - 2 * (z * z);
			matrix.md[0][1] = 2 * x * y + 2 * w * z;
			matrix.md[0][2] = 2 * x * z - 2 * w * y;

			matrix.md[1][0] = 2 * x * y - 2 * w * y;
			matrix.md[1][1] = 1 - 2 * x * x - 2 * z * z;
			matrix.md[1][2] = 2 * y * z + 2 * w * x;

			matrix.md[2][0] = 2 * x * z + 2 * w * y;
			matrix.md[2][1] = 2 * y * z - 2 * w * x;
			matrix.md[2][2] = 1 - 2 * x * x - 2 * y * y;*/

			matrix.md[3][3] = 1.0f;

			return matrix;
		}

		inline Quaternion& Normalize()
		{
			// compute the magnitude and divide through it
			const float mag = std::sqrt(x * x + y * y + z * z + w * w);
			if (mag)
			{
				const float invMag = static_cast<float>(1.0) / mag;
				x *= invMag;
				y *= invMag;
				z *= invMag;
				w *= invMag;
			}
			return *this;
		}
	};
}

#endif