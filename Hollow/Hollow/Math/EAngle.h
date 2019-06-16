#pragma once

#ifndef HW_E_ANGLE_H
#define HW_E_ANGLE_H

#include "Vector4.h"
#include <math.h>

namespace Hollow {
	class EAngle
	{
	public:
		float pitch;
		float yaw;
		float roll;
	public:
		EAngle()
		{
			pitch = 0.0f;
			yaw = 0.0f;
			roll = 0.0f;
		}

		EAngle(float roll, float pitch, float yaw) :
			roll(roll), pitch(pitch), yaw(yaw)
		{}

		Vector4 toVector()
		{
			float y2 = yaw / 360 * 2 * (float)Math::PI;
			float p2 = pitch / 360 * 2 * (float)Math::PI;

			float x = cosf(y2) * cos(p2);
			float y = sin(p2);
			float z = sinf(y2) * cos(p2);

			return Vector4(x, y, z, 1.0f);
		}

		void Normalize()
		{
			if (pitch > 89)
				pitch = 89;
			if (pitch < -89)
				pitch = -89;

			while (yaw < -180)
				yaw += 360;

			while (yaw > 180)
				yaw -= 360;
		}
	};
}

#endif