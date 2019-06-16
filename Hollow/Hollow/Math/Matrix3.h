#pragma once

#ifndef HW_MATRIX_3_H
#define HW_MATRIX_3_H

namespace Hollow {
	class Matrix3
	{
	public:
		float data[9];
	public:
		Matrix3()
		{
			for (int i = 0; i < 9; i++)
				data[i] = 0;
		}

		Matrix3& Identity()
		{
			Matrix3 matrix;

			matrix.data[0] = 1.0f;
			matrix.data[4] = 1.0f;
			matrix.data[8] = 1.0f;

			return matrix;
		}

		Matrix3& operator*(Matrix3& other)
		{
			Matrix3 temp;

			temp.data[0] = data[0] * other.data[0] + data[1] * other.data[3] + data[2] * other.data[6];
			temp.data[1] = data[0] * other.data[1] + data[1] * other.data[4] + data[2] * other.data[7];
			temp.data[2] = data[0] * other.data[2] + data[1] * other.data[5] + data[2] * other.data[8];

			temp.data[3] = data[3] * other.data[0] + data[4] * other.data[3] + data[5] * other.data[6];
			temp.data[4] = data[3] * other.data[1] + data[4] * other.data[4] + data[5] * other.data[7];
			temp.data[5] = data[3] * other.data[2] + data[4] * other.data[5] + data[5] * other.data[8];

			temp.data[6] = data[6] * other.data[0] + data[7] * other.data[3] + data[8] * other.data[6];
			temp.data[7] = data[6] * other.data[1] + data[7] * other.data[4] + data[8] * other.data[7];
			temp.data[8] = data[6] * other.data[2] + data[7] * other.data[5] + data[8] * other.data[8];

			return temp;
		}

		Matrix3& operator*(int val)
		{
			for (int i = 0; i < 9; i++)
			{
				data[i] *= val;
			}
			return *this;
		}
	};
}

#endif