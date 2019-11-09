#pragma once

#ifndef HW_QUATERNION_H
#define HW_QUATERNION_H

#include "Math.h"
#include <cmath>
#include "Matrix4.h"
#include "Vector3.h"

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
			x(0), y(0), z(0), w(1)
		{}

		Quaternion(float x, float y, float z, float w) :
			x(x), y(y), z(z), w(w)
		{}

		// From euler angles
		Quaternion(const Vector3& vec) 
		{
			double cy = cos(vec.z * 0.5);
			double sy = sin(vec.z * 0.5);
			double cp = cos(vec.y * 0.5);
			double sp = sin(vec.y * 0.5);
			double cr = cos(vec.x * 0.5);
			double sr = sin(vec.x * 0.5);

			w = cy * cp * cr + sy * sp * sr;
			x = cy * cp * sr - sy * sp * cr;
			y = sy * cp * sr + cy * sp * cr;
			z = sy * cp * cr - cy * sp * sr;
		}

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

		static inline void interpolate(Quaternion& pOut, const Quaternion& pStart, const Quaternion& pEnd, float pFactor)
		{
			/*
			float a = 1.0 - t, b = t, d = pStart.x * pEnd.x + pStart.y * pEnd.y + pStart.z * pEnd.z + pStart.w * pEnd.w, c = fabsf(d);
			if (c < 0.999) {
				c = acosf(c);
				b = 1 / sinf(c);
				a = sinf(a * c) * b;
				b *= sinf(t * c);
				if (d < 0) b = -b;
			}
			
			pOut.x = a * pStart.x + b * pEnd.x;
			pOut.y = a * pStart.y + b * pEnd.y;
			pOut.z = a * pStart.z + b * pEnd.z;
			pOut.w = a * pStart.w + b * pEnd.w;
			*/
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

		inline Quaternion& normalize()
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