#include "Matrix4.h"

namespace Hollow {
	Matrix4::Matrix4()
	{
		for (int i = 0; i < 16; i++)
			m[i] = 0;
	}

	Matrix4::Matrix4(const Matrix4& other)
	{
		for (int i = 0; i < 16; i++)
			m[i] = other.m[i];
	}

	Matrix4::Matrix4(Matrix4&& other)
	{
		for (int i = 0; i < 16; i++)
			m[i] = other.m[i];
	}

	Matrix4::Matrix4(const float* other, int size)
	{
		Matrix4 identity = Matrix4::identity();

		switch (size)
		{
		case 16:
			for (int i = 0; i < size; i++) m[i] = other[i];
			break;
		case 9:
		{
			for (int i = 0; i < 16; i++)
				m[i] = 0;
			m[15] = 1.0f;
			int counter = 0;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					md[i][j] = other[counter++];
				}
			}
		} break;
		default:
			break;
		}
	}

	Matrix4::Matrix4(const double* other, int size)
	{
		Matrix4 identity = Matrix4::identity();

		switch (size)
		{
		case 16:
			for (int i = 0; i < size; i++) m[i] = (float)other[i];
			break;
		case 9:
		{
			for (int i = 0; i < 16; i++)
				m[i] = 0;
			m[15] = 1.0f;
			int counter = 0;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					md[i][j] = (float)other[counter++];
				}
			}
		} break;
		default:
			break;
		}
	}

	Matrix4& Matrix4::operator=(const Matrix4& other)
	{
		for (int i = 0; i < 16; i++)
			m[i] = other.m[i];
		return *this;
	}

	Matrix4& Matrix4::operator=(Matrix4&& other)
	{
		for (int i = 0; i < 16; i++)
			m[i] = other.m[i];
		return *this;
	}

	Matrix4 Matrix4::identity()
	{
		Matrix4 matrix;

		matrix.md[0][0] = 1.0f;
		matrix.md[1][1] = 1.0f;
		matrix.md[2][2] = 1.0f;
		matrix.md[3][3] = 1.0f;

		return matrix;
	}

	Matrix4 Matrix4::translation(float x, float y, float z)
	{
		Matrix4 matrix = identity();

		matrix.md[3][0] = x;
		matrix.md[3][1] = y;
		matrix.md[3][2] = z;

		return matrix;
	}

	Matrix4 Matrix4::translation(const Vector4& vec)
	{
		return translation(vec.x, vec.y, vec.z);
	}

	Matrix4 Matrix4::scaling(float x, float y, float z)
	{
		Matrix4 matrix;

		matrix.md[0][0] = x;
		matrix.md[1][1] = y;
		matrix.md[2][2] = z;
		matrix.md[3][3] = 1.0f;

		return matrix;
	}

	Matrix4 Matrix4::scaling(const Vector4& vec)
	{
		return scaling(vec.x, vec.y, vec.z);
	}

	Matrix4 Matrix4::rotation(const Vector4& vec)
	{
		return rotation(vec.x, vec.y, vec.z);
	}

	Matrix4 Matrix4::rotation(float x, float y, float z)
	{
		Matrix4 M = rotationX(x);
		Matrix4 M1 = rotationY(y);
		Matrix4 M2 = rotationZ(z);

		return M * M2 * M1;
	}

	Matrix4 Matrix4::rotationX(float x)
	{
		Matrix4 matrix = identity();

		matrix.md[1][1] = cosf(x);
		matrix.md[1][2] = -sinf(x);
		matrix.md[2][1] = sinf(x);
		matrix.md[2][2] = cosf(x);

		return matrix;
	}

	Matrix4 Matrix4::rotationY(float y)
	{
		Matrix4 matrix = identity();

		matrix.md[0][0] = cosf(y);
		matrix.md[0][2] = sinf(y);
		matrix.md[2][0] = -sinf(y);
		matrix.md[2][2] = cosf(y);

		return matrix;
	}

	Matrix4 Matrix4::rotationZ(float z)
	{
		Matrix4 matrix = identity();

		matrix.md[0][0] = cosf(z);
		matrix.md[0][1] = -sinf(z);
		matrix.md[1][0] = sinf(z);
		matrix.md[1][1] = cosf(z);

		return matrix;
	}

	Matrix4& Matrix4::transpose()
	{
		float temp;

		temp = md[0][1];
		md[0][1] = md[1][0];
		md[1][0] = temp;

		temp = md[0][2];
		md[0][2] = md[2][0];
		md[2][0] = temp;

		temp = md[0][3];
		md[0][3] = md[3][0];
		md[3][0] = temp;

		temp = md[1][2];
		md[1][2] = md[2][1];
		md[2][1] = temp;

		temp = md[1][3];
		md[1][3] = md[3][1];
		md[3][1] = temp;

		temp = md[2][3];
		md[2][3] = md[3][2];
		md[3][2] = temp;

		return *this;
	}

	Matrix4 Matrix4::transpose(const Matrix4& matrix)
	{
		Matrix4 result;

		result.md[0][0] = matrix.md[0][0];
		result.md[0][1] = matrix.md[1][0];
		result.md[0][2] = matrix.md[2][0];
		result.md[0][3] = matrix.md[3][0];

		result.md[1][0] = matrix.md[0][1];
		result.md[1][1] = matrix.md[1][1];
		result.md[1][2] = matrix.md[2][1];
		result.md[1][3] = matrix.md[3][1];

		result.md[2][0] = matrix.md[0][2];
		result.md[2][1] = matrix.md[1][2];
		result.md[2][2] = matrix.md[2][2];
		result.md[2][3] = matrix.md[3][2];

		result.md[3][0] = matrix.md[0][3];
		result.md[3][1] = matrix.md[1][3];
		result.md[3][2] = matrix.md[2][3];
		result.md[3][3] = matrix.md[3][3];

		return result;
	}

	Matrix4 Matrix4::operator*(const Matrix4& other) const
	{
		Matrix4 matrix;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				matrix.md[i][j] = (md[i][0] * other.md[0][j]) + (md[i][1] * other.md[1][j]) + (md[i][2] * other.md[2][j]) + (md[i][3] * other.md[3][j]);
			}
		}

		return matrix;
	}

	Matrix4 Matrix4::projection(float fov, float aspect, float n, float f)
	{
		Matrix4 projection;

		float temp = Math::cotan(fov / 2);

		projection.md[0][0] = temp / aspect;
		projection.md[1][1] = temp;
		projection.md[2][2] = (f + n) / (f - n);
		projection.md[3][2] = 1.0f;
		projection.md[2][3] = -((f * n) / (f - n));
		projection.md[3][3] = 0;

		return projection;
	}

	Matrix4 Matrix4::orthographic(float right, float left, float top, float bottom, float near, float far)
	{
		Matrix4 ortho;

		ortho.md[0][0] = 2 / (right - left);
		ortho.md[1][1] = 2 / (top - bottom);
		ortho.md[2][2] = -2 / (far - near);

		ortho.md[0][3] = - ((right + left) / (right - left));
		ortho.md[1][3] = -((top + bottom) / (top - bottom));
		ortho.md[2][3] = -((far + near) / (far - near));
		ortho.md[3][3] = 1.0f;

		return ortho;
	}

	Matrix4 Matrix4::lookAt(const Vector4& eyePosition, const Vector4& eyeDirection, const Vector4& upVector)
	{
		Vector4 forward = Vector4::normalize(eyeDirection - eyePosition);
		Vector4 right = Vector4::normalize(Vector4::cross(upVector, forward));
		Vector4 up = Vector4::normalize(Vector4::cross(forward, right));

		Matrix4 camToWorld;

		Vector4 negativePosition = Vector4::negate(eyePosition);

		camToWorld.md[0][0] = right.x;
		camToWorld.md[0][1] = right.y;
		camToWorld.md[0][2] = right.z;
		camToWorld.md[0][3] = Vector4::dot(right, negativePosition);

		camToWorld.md[1][0] = up.x;
		camToWorld.md[1][1] = up.y;
		camToWorld.md[1][2] = up.z;
		camToWorld.md[1][3] = Vector4::dot(up, negativePosition);

		camToWorld.md[2][0] = forward.x;
		camToWorld.md[2][1] = forward.y;
		camToWorld.md[2][2] = forward.z;
		camToWorld.md[2][3] = Vector4::dot(forward, negativePosition);

		camToWorld.md[3][0] = 0.0f;
		camToWorld.md[3][1] = 0.0f;
		camToWorld.md[3][2] = 0.0f;
		camToWorld.md[3][3] = 1.0f;

		return camToWorld;
	}


	void Matrix4::setTranslation(const Vector4& vecPos)
	{
		md[3][0] = vecPos.x;
		md[3][1] = vecPos.y;
		md[3][2] = vecPos.z;
	}

	Vector4 Matrix4::getTranslation() const
	{
		return Vector4(md[3][0], md[3][1], md[3][2], 0.0f);
	}

	Matrix4 Matrix4::inverse(const Matrix4& mat)
	{
		float temp[16];

		temp[0] = mat.m[5] * mat.m[10] * mat.m[15] -
			mat.m[5] * mat.m[11] * mat.m[14] -
			mat.m[9] * mat.m[6] * mat.m[15] +
			mat.m[9] * mat.m[7] * mat.m[14] +
			mat.m[13] * mat.m[6] * mat.m[11] -
			mat.m[13] * mat.m[7] * mat.m[10];

		temp[4] = -mat.m[4] * mat.m[10] * mat.m[15] +
			mat.m[4] * mat.m[11] * mat.m[14] +
			mat.m[8] * mat.m[6] * mat.m[15] -
			mat.m[8] * mat.m[7] * mat.m[14] -
			mat.m[12] * mat.m[6] * mat.m[11] +
			mat.m[12] * mat.m[7] * mat.m[10];

		temp[8] = mat.m[4] * mat.m[9] * mat.m[15] -
			mat.m[4] * mat.m[11] * mat.m[13] -
			mat.m[8] * mat.m[5] * mat.m[15] +
			mat.m[8] * mat.m[7] * mat.m[13] +
			mat.m[12] * mat.m[5] * mat.m[11] -
			mat.m[12] * mat.m[7] * mat.m[9];

		temp[12] = -mat.m[4] * mat.m[9] * mat.m[14] +
			mat.m[4] * mat.m[10] * mat.m[13] +
			mat.m[8] * mat.m[5] * mat.m[14] -
			mat.m[8] * mat.m[6] * mat.m[13] -
			mat.m[12] * mat.m[5] * mat.m[10] +
			mat.m[12] * mat.m[6] * mat.m[9];

		temp[1] = -mat.m[1] * mat.m[10] * mat.m[15] +
			mat.m[1] * mat.m[11] * mat.m[14] +
			mat.m[9] * mat.m[2] * mat.m[15] -
			mat.m[9] * mat.m[3] * mat.m[14] -
			mat.m[13] * mat.m[2] * mat.m[11] +
			mat.m[13] * mat.m[3] * mat.m[10];

		temp[5] = mat.m[0] * mat.m[10] * mat.m[15] -
			mat.m[0] * mat.m[11] * mat.m[14] -
			mat.m[8] * mat.m[2] * mat.m[15] +
			mat.m[8] * mat.m[3] * mat.m[14] +
			mat.m[12] * mat.m[2] * mat.m[11] -
			mat.m[12] * mat.m[3] * mat.m[10];

		temp[9] = -mat.m[0] * mat.m[9] * mat.m[15] +
			mat.m[0] * mat.m[11] * mat.m[13] +
			mat.m[8] * mat.m[1] * mat.m[15] -
			mat.m[8] * mat.m[3] * mat.m[13] -
			mat.m[12] * mat.m[1] * mat.m[11] +
			mat.m[12] * mat.m[3] * mat.m[9];

		temp[13] = mat.m[0] * mat.m[9] * mat.m[14] -
			mat.m[0] * mat.m[10] * mat.m[13] -
			mat.m[8] * mat.m[1] * mat.m[14] +
			mat.m[8] * mat.m[2] * mat.m[13] +
			mat.m[12] * mat.m[1] * mat.m[10] -
			mat.m[12] * mat.m[2] * mat.m[9];

		temp[2] = mat.m[1] * mat.m[6] * mat.m[15] -
			mat.m[1] * mat.m[7] * mat.m[14] -
			mat.m[5] * mat.m[2] * mat.m[15] +
			mat.m[5] * mat.m[3] * mat.m[14] +
			mat.m[13] * mat.m[2] * mat.m[7] -
			mat.m[13] * mat.m[3] * mat.m[6];

		temp[6] = -mat.m[0] * mat.m[6] * mat.m[15] +
			mat.m[0] * mat.m[7] * mat.m[14] +
			mat.m[4] * mat.m[2] * mat.m[15] -
			mat.m[4] * mat.m[3] * mat.m[14] -
			mat.m[12] * mat.m[2] * mat.m[7] +
			mat.m[12] * mat.m[3] * mat.m[6];

		temp[10] = mat.m[0] * mat.m[5] * mat.m[15] -
			mat.m[0] * mat.m[7] * mat.m[13] -
			mat.m[4] * mat.m[1] * mat.m[15] +
			mat.m[4] * mat.m[3] * mat.m[13] +
			mat.m[12] * mat.m[1] * mat.m[7] -
			mat.m[12] * mat.m[3] * mat.m[5];

		temp[14] = -mat.m[0] * mat.m[5] * mat.m[14] +
			mat.m[0] * mat.m[6] * mat.m[13] +
			mat.m[4] * mat.m[1] * mat.m[14] -
			mat.m[4] * mat.m[2] * mat.m[13] -
			mat.m[12] * mat.m[1] * mat.m[6] +
			mat.m[12] * mat.m[2] * mat.m[5];

		temp[3] = -mat.m[1] * mat.m[6] * mat.m[11] +
			mat.m[1] * mat.m[7] * mat.m[10] +
			mat.m[5] * mat.m[2] * mat.m[11] -
			mat.m[5] * mat.m[3] * mat.m[10] -
			mat.m[9] * mat.m[2] * mat.m[7] +
			mat.m[9] * mat.m[3] * mat.m[6];

		temp[7] = mat.m[0] * mat.m[6] * mat.m[11] -
			mat.m[0] * mat.m[7] * mat.m[10] -
			mat.m[4] * mat.m[2] * mat.m[11] +
			mat.m[4] * mat.m[3] * mat.m[10] +
			mat.m[8] * mat.m[2] * mat.m[7] -
			mat.m[8] * mat.m[3] * mat.m[6];

		temp[11] = -mat.m[0] * mat.m[5] * mat.m[11] +
			mat.m[0] * mat.m[7] * mat.m[9] +
			mat.m[4] * mat.m[1] * mat.m[11] -
			mat.m[4] * mat.m[3] * mat.m[9] -
			mat.m[8] * mat.m[1] * mat.m[7] +
			mat.m[8] * mat.m[3] * mat.m[5];

		temp[15] = mat.m[0] * mat.m[5] * mat.m[10] -
			mat.m[0] * mat.m[6] * mat.m[9] -
			mat.m[4] * mat.m[1] * mat.m[10] +
			mat.m[4] * mat.m[2] * mat.m[9] +
			mat.m[8] * mat.m[1] * mat.m[6] -
			mat.m[8] * mat.m[2] * mat.m[5];

		float determinant = mat.m[0] * temp[0] + mat.m[1] * temp[4] + mat.m[2] * temp[8] + mat.m[3] * temp[12];
		determinant = 1.0f / determinant;

		Matrix4 tempM;

		for (int i = 0; i < 4 * 4; i++)
			tempM.m[i] = temp[i] * determinant;

		return tempM;
	}


	Vector4 operator*(const Vector4& vec, const Matrix4& mat)
	{
		float x = vec.x * mat.md[0][0] + vec.y * mat.md[1][0] + vec.z * mat.md[2][0] + vec.w * mat.md[3][0];
		float y = vec.x * mat.md[0][1] + vec.y * mat.md[1][1] + vec.z * mat.md[2][1] + vec.w * mat.md[3][1];
		float z = vec.x * mat.md[0][2] + vec.y * mat.md[1][2] + vec.z * mat.md[2][2] + vec.w * mat.md[3][2];
		float w = vec.x * mat.md[0][3] + vec.y * mat.md[1][3] + vec.z * mat.md[2][3] + vec.w * mat.md[3][3];

		return Vector4(x, y, z, w);
	}

	Vector4 operator*(const Matrix4& mat, const Vector4& vec)
	{
		return vec * mat;
	}

	Vector3 operator*(const Vector3& vec, const Matrix4& mat)
	{
		float x = vec.x * mat.md[0][0] + vec.y * mat.md[1][0] + vec.z * mat.md[2][0] + mat.md[3][0];
		float y = vec.x * mat.md[0][1] + vec.y * mat.md[1][1] + vec.z * mat.md[2][1] + mat.md[3][1];
		float z = vec.x * mat.md[0][2] + vec.y * mat.md[1][2] + vec.z * mat.md[2][2] + mat.md[3][2];

		return Vector3(x, y, z);
	}

	Vector3 operator*(const Matrix4& mat, const Vector3& vec)
	{
		return vec * mat;
	}
}