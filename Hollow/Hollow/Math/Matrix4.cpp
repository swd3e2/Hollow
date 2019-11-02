#include "Matrix4.h"

#define USE_SIMD 1

namespace Hollow {
	Matrix4::Matrix4(float _a1, float _a2, float _a3, float _a4, 
					 float _b1, float _b2, float _b3, float _b4, 
					 float _c1, float _c2, float _c3, float _c4,
					 float _d1, float _d2, float _d3, float _d4)
	{
		r[0] = Vector4(_a1, _a2, _a3, _a4);
		r[1] = Vector4(_b1, _b2, _b3, _b4);
		r[2] = Vector4(_c1, _c2, _c3, _c4);
		r[3] = Vector4(_d1, _d2, _d3, _d4);
	}

	Matrix4::Matrix4(const Matrix4& other)
	{
		r[0] = other.r[0];
		r[1] = other.r[1];
		r[2] = other.r[2];
		r[3] = other.r[3];
	}

	Matrix4::Matrix4(const float* other, int size)
	{
		switch (size)
		{
		case 16:
			r[0] = Vector4(other[0],  other[1],  other[2],  other[3]);
			r[1] = Vector4(other[4],  other[5],  other[6],  other[7]);
			r[2] = Vector4(other[8],  other[9],  other[10], other[11]);
			r[3] = Vector4(other[12], other[13], other[14], other[15]);
		case 9:
		{
			r[0] = Vector4(other[0], other[1], other[2], 0);
			r[1] = Vector4(other[3], other[4], other[5], 0);
			r[2] = Vector4(other[6], other[7], other[8], 0);
			r[3] = Vector4(0, 0, 0, 0);
		} break;
		default:
			break;
		}
	}

	Matrix4& Matrix4::operator=(const Matrix4& other)
	{
		r[0] = other.r[0];
		r[1] = other.r[1];
		r[2] = other.r[2];
		r[3] = other.r[3];
		return *this;
	}

	Matrix4 Matrix4::identity()
	{
		return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
					   0.0f, 1.0f, 0.0f, 0.0f,
					   0.0f, 0.0f, 1.0f, 0.0f,
					   0.0f, 0.0f, 0.0f, 1.0f);
	}

	Matrix4 Matrix4::translation(float x, float y, float z)
	{
		return Matrix4(1.0f, 0.0f, 0.0f, x,
					   0.0f, 1.0f, 0.0f, y,
					   0.0f, 0.0f, 1.0f, z,
					   0.0f, 0.0f, 0.0f, 1.0f);
	}

	Matrix4 Matrix4::translation(const Vector4& vec)
	{
		return Matrix4(1.0f, 0.0f, 0.0f, vec.x,
					   0.0f, 1.0f, 0.0f, vec.y,
					   0.0f, 0.0f, 1.0f, vec.z,
					   0.0f, 0.0f, 0.0f, 1.0f);
	}

	Matrix4 Matrix4::scaling(float x, float y, float z)
	{
		return Matrix4(x,    0.0f, 0.0f, 0.0f,
			           0.0f, y,    0.0f, 0.0f,
			           0.0f, 0.0f, z,    0.0f,
			           0.0f, 0.0f, 0.0f, 1.0f);
	}

	Matrix4 Matrix4::scaling(const Vector4& vec)
	{
		return Matrix4(vec.x, 0.0f,  0.0f,  0.0f,
			           0.0f,  vec.y, 0.0f,  0.0f,
			           0.0f,  0.0f,  vec.z, 0.0f,
			           0.0f,  0.0f,  0.0f,  1.0f);
	}

	Matrix4 Matrix4::rotation(const Vector4& vec)
	{
		return rotation(vec.x, vec.y, vec.z);
	}

	Matrix4 Matrix4::rotation(float x, float y, float z)
	{
		return rotationX(x) * rotationY(y) * rotationZ(z);
	}

	Matrix4 Matrix4::rotation(const Quaternion& quat)
	{
		const float n = 1.0f / sqrt(quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w);
		float x = quat.x * n;
		float y = quat.y * n;
		float z = quat.z * n;
		float w = quat.w * n;

		return Matrix4(1 - 2 * (y * y) - 2 * (z * z),	2 * x * y - 2 * z * w,			2 * x * z + 2 * y * w,			0.0f,
					   2 * x * y + 2 * z * w,			1 - 2 * (x * x) - 2 * (z * z),	2 * y * z - 2 * x * w,			0.0f,
					   2 * x * z - 2 * y * w,			2 * y * z + 2 * x * w,			1 - 2 * (x * x) - 2 * (y * y),	0.0f,
					   0.0f,							0.0f,							0.0f,							1.0f);
	}

	Matrix4 Matrix4::rotationX(float x)
	{
		return Matrix4(1.0f, 0.0f,	  0.0f,		0.0f,
			           0.0f, cosf(x), -sinf(x), 0.0f,
			           0.0f, sinf(x), cosf(x),	0.0f,
			           0.0f, 0.0f,    0.0f,		1.0f);
	}

	Matrix4 Matrix4::rotationY(float y)
	{
		return Matrix4(cosf(y),  0.0f, sinf(y), 0.0f,
			           0.0f,	 1.0f, 0.0f,	0.0f,
			           -sinf(y), 0.0f, cosf(y), 0.0f,
			           0.0f,     0.0f, 0.0f,	1.0f);
	}

	Matrix4 Matrix4::rotationZ(float z)
	{
		return Matrix4(cosf(z), -sinf(z), 0.0f, 0.0f,
					   sinf(z), cosf(z),  0.0f, 0.0f,
					   0.0f,	0.0f,	  1.0f, 0.0f,
					   0.0f,	0.0f,     0.0f, 1.0f);
	}

	Matrix4& Matrix4::transpose()
	{
		swap(r[0].y, r[1].x);
		swap(r[0].z, r[2].x);
		swap(r[0].w, r[3].x);
		swap(r[1].z, r[2].y);
		swap(r[1].w, r[3].y);
		swap(r[2].w, r[3].z);

		return *this;
	}

	Matrix4 Matrix4::transpose(const Matrix4& matrix)
	{
		return Matrix4(matrix.r[0].x, matrix.r[1].x, matrix.r[2].x, matrix.r[3].x,
					   matrix.r[0].y, matrix.r[1].y, matrix.r[2].y, matrix.r[3].y,
				       matrix.r[0].z, matrix.r[1].z, matrix.r[2].z, matrix.r[3].z,
					   matrix.r[0].w, matrix.r[1].w, matrix.r[2].w, matrix.r[3].w);
	}

	Matrix4 Matrix4::operator*(const Matrix4& other) const
	{
		Matrix4 result;

#ifndef USE_SIMD
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				matrix.md[i][j] = (md[i][0] * other.md[0][j]) + (md[i][1] * other.md[1][j]) + (md[i][2] * other.md[2][j]) + (md[i][3] * other.md[3][j]);
			}
		}
#else
		__m128 vW = *r[0];
		__m128 vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

		vX = _mm_mul_ps(vX, *other.r[0]);
		vY = _mm_mul_ps(vY, *other.r[1]);
		vZ = _mm_mul_ps(vZ, *other.r[2]);
		vW = _mm_mul_ps(vW, *other.r[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		result.r[0] = vX;

		vW = *r[1];
		vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

		vX = _mm_mul_ps(vX, *other.r[0]);
		vY = _mm_mul_ps(vY, *other.r[1]);
		vZ = _mm_mul_ps(vZ, *other.r[2]);
		vW = _mm_mul_ps(vW, *other.r[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		result.r[1] = vX;

		vW = *r[2];
		vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

		vX = _mm_mul_ps(vX, *other.r[0]);
		vY = _mm_mul_ps(vY, *other.r[1]);
		vZ = _mm_mul_ps(vZ, *other.r[2]);
		vW = _mm_mul_ps(vW, *other.r[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		result.r[2] = vX;

		vW = *r[3];
		vX = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(0, 0, 0, 0));
		vY = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(1, 1, 1, 1));
		vZ = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(2, 2, 2, 2));
		vW = _mm_shuffle_ps(vW, vW, _MM_SHUFFLE(3, 3, 3, 3));

		vX = _mm_mul_ps(vX, *other.r[0]);
		vY = _mm_mul_ps(vY, *other.r[1]);
		vZ = _mm_mul_ps(vZ, *other.r[2]);
		vW = _mm_mul_ps(vW, *other.r[3]);
		vX = _mm_add_ps(vX, vZ);
		vY = _mm_add_ps(vY, vW);
		vX = _mm_add_ps(vX, vY);
		result.r[3] = vX;
#endif

		return result;
	}

	Matrix4 Matrix4::projection(float fov, float aspect, float n, float f)
	{
		float temp = Math::cotan(fov / 2);

		return Matrix4(temp / aspect, 0.0f, 0.0f,              0.0f,
					   0.0f,		 temp,  0.0f,              0.0f,
					   0.0f,		 0.0f,  (f + n) / (f - n), -((f * n) / (f - n)),
					   0.0f,		 0.0f,  1.0f,			   0.0f);
	}

	Matrix4 Matrix4::orthographic(float right, float left, float top, float bottom, float near, float far)
	{
		return Matrix4(2 / (right - left),	0.0f,				0.0f,				-((right + left) / (right - left)),
					   0.0f,				2 / (top - bottom), 0.0f,				-((top + bottom) / (top - bottom)),
					   0.0f,				0.0f,				-2 / (far - near),	-((far + near) / (far - near)),
					   0.0f,				0.0f,				0.0f,				1.0f);
	}

	Matrix4 Matrix4::lookAt(const Vector4& eyePosition, const Vector4& eyeDirection, const Vector4& upVector)
	{
		Vector4 forward = Vector4::normalize(eyeDirection - eyePosition);
		Vector4 right = Vector4::normalize(Vector4::cross(upVector, forward));
		Vector4 up = Vector4::normalize(Vector4::cross(forward, right));

		Vector4 negativePosition = Vector4::negate(eyePosition);

		return Matrix4(right.x,		right.y,	right.z,	Vector4::dot(right, negativePosition),
					   up.x,		up.y,		up.z,		Vector4::dot(up, negativePosition),
					   forward.x,	forward.y,	forward.z,	Vector4::dot(forward, negativePosition),
					   0.0f,		0.0f,		0.0f,		1.0f);
	}

	Matrix4 Matrix4::inverse(const Matrix4& mat)
	{
		float temp[16];

		/*temp[0] = mat.m[5] * mat.m[10] * mat.m[15] -
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
			tempM.m[i] = temp[i] * determinant;*/

		return Matrix4();
	}

	Vector4 operator*(const Vector4& vec, const Matrix4& mat)
	{
		float x = vec.x * mat.r[0].x + vec.y * mat.r[1].x + vec.z * mat.r[2].x + vec.w * mat.r[3].x;
		float y = vec.x * mat.r[0].y + vec.y * mat.r[1].y + vec.z * mat.r[2].y + vec.w * mat.r[3].y;
		float z = vec.x * mat.r[0].z + vec.y * mat.r[1].z + vec.z * mat.r[2].z + vec.w * mat.r[3].z;
		float w = vec.x * mat.r[0].w + vec.y * mat.r[1].w + vec.z * mat.r[2].w + vec.w * mat.r[3].w;

		return Vector4(x, y, z, w);
	}

	Vector4 operator*(const Matrix4& mat, const Vector4& vec)
	{
		return vec * mat;
	}

	Vector3 operator*(const Vector3& vec, const Matrix4& mat)
	{
		float x = vec.x * mat.r[0].x + vec.y * mat.r[1].x + vec.z * mat.r[2].x;
		float y = vec.x * mat.r[0].y + vec.y * mat.r[1].y + vec.z * mat.r[2].y;
		float z = vec.x * mat.r[0].z + vec.y * mat.r[1].z + vec.z * mat.r[2].z;

		return Vector3(x, y, z);
	}

	Vector3 operator*(const Matrix4& mat, const Vector3& vec)
	{
		return vec * mat;
	}
}