#include "Matrix4.h"

Vector4 operator*(const Vector4 & vec, const Matrix4 & mat)
{
	float x = vec.x * mat.md[0][0] + vec.x * mat.md[1][0] + vec.x * mat.md[2][0] + vec.x * mat.md[3][0];
	float y = vec.y * mat.md[0][1] + vec.y * mat.md[1][1] + vec.y * mat.md[2][1] + vec.y * mat.md[3][1];
	float z = vec.z * mat.md[0][2] + vec.z * mat.md[1][2] + vec.z * mat.md[2][2] + vec.z * mat.md[3][2];
	float w = vec.w * mat.md[0][3] + vec.w * mat.md[1][3] + vec.w * mat.md[2][3] + vec.w * mat.md[3][3];

	return Vector4(x, y, z, w);
}

Matrix4 operator*(const Matrix4& left, const Matrix4 & right)
{
	Matrix4 temp = left;
	temp = temp * right;
	return temp;
}

Matrix4::Matrix4()
{
	for (int i = 0; i < 16; i++)
		m[i] = 0;
}

Matrix4::Matrix4(const Matrix4 & other)
{
	for (int i = 0; i < 16; i++)
		m[i] = other.m[i];
}

Matrix4::Matrix4(Matrix4 && other)
{
	for (int i = 0; i < 16; i++)
		m[i] = other.m[i];
}

Matrix4 & Matrix4::operator=(const Matrix4& other)
{
	for (int i = 0; i < 16; i++)
		m[i] = other.m[i];
	return *this;
}

Matrix4 & Matrix4::operator=(Matrix4 && other)
{
	for (int i = 0; i < 16; i++)
		m[i] = other.m[i];
	return *this;
}

Matrix4 Matrix4::Identity()
{
	Matrix4 matrix;
	matrix.m[0] = 1.0f;
	matrix.m[5] = 1.0f;
	matrix.m[10] = 1.0f;
	matrix.m[15] = 1.0f;
	return matrix;
}

Matrix4 Matrix4::Translation(float x, float y, float z)
{
	Matrix4 matrix;

	matrix.m[0] = 1.0f;
	matrix.m[5] = 1.0f;
	matrix.m[10] = 1.0f;

	matrix.m[12] = x;
	matrix.m[13] = y;
	matrix.m[14] = z;

	matrix.m[15] = 1.0f;

	return std::move(matrix);
}

Matrix4 Matrix4::Scaling(float x, float y, float z)
{
	Matrix4 matrix;

	matrix.m[0] = x;
	matrix.m[5] = y;
	matrix.m[10] = z;
	matrix.m[15] = 1.0f;

	return std::move(matrix);
}

Matrix4 Matrix4::Rotation(float x, float y, float z)
{
	Matrix4 M = RotationX(x);
	Matrix4 M1 = RotationY(y);
	Matrix4 M2 = RotationZ(z);

	return M * M1 * M2;
}

Matrix4 Matrix4::RotationX(float x)
{
	Matrix4 matrix;

	matrix.m[0] = 1.0f;
	matrix.m[15] = 1.0f;

	matrix.m[5] = cosf(x);
	matrix.m[6] = -sinf(x);
	matrix.m[9] = sinf(x);
	matrix.m[10] = cosf(x);

	return std::move(matrix);
}

Matrix4 Matrix4::RotationY(float y)
{
	Matrix4 matrix;

	matrix.m[5] = 1.0f;
	matrix.m[15] = 1.0f;

	matrix.m[0] = cosf(y);
	matrix.m[2] = sinf(y);
	matrix.m[8] = -sinf(y);
	matrix.m[10] = cosf(y);

	return std::move(matrix);
}

Matrix4 Matrix4::RotationZ(float z)
{
	Matrix4 matrix;

	matrix.m[10] = 1.0f;
	matrix.m[15] = 1.0f;

	matrix.m[0] = cosf(z);
	matrix.m[1] = -sinf(z);
	matrix.m[4] = sinf(z);
	matrix.m[5] = cosf(z);

	return std::move(matrix);
}

Matrix4& Matrix4::Transpose()
{
	float temp;

	temp = m[1];
	m[1] = m[4];
	m[4] = temp;

	temp = m[2];
	m[2] = m[8];
	m[8] = temp;

	temp = m[3];
	m[3] = m[12];
	m[12] = temp;

	temp = m[6];
	m[6] = m[9];
	m[9] = temp;

	temp = m[7];
	m[7] = m[13];
	m[13] = temp;

	temp = m[11];
	m[11] = m[14];
	m[14] = temp;

	return *this;
}

Matrix4 Matrix4::Transpose(const Matrix4& matrix)
{
	Matrix4 result;

	float temp;

	result.md[0][0] = matrix.md[0][0];
	result.md[1][1] = matrix.md[1][1];
	result.md[2][2] = matrix.md[2][2];
	result.md[3][3] = matrix.md[3][3];

	temp = matrix.m[1];
	result.m[1] = matrix.m[4];
	result.m[4] = temp;

	temp = matrix.m[2];
	result.m[2] = matrix.m[8];
	result.m[8] = temp;

	temp = matrix.m[3];
	result.m[3] = matrix.m[12];
	result.m[12] = temp;

	temp = matrix.m[6];
	result.m[6] = matrix.m[9];
	result.m[9] = temp;

	temp = matrix.m[7];
	result.m[7] = matrix.m[13];
	result.m[13] = temp;

	temp = matrix.m[11];
	result.m[11] = matrix.m[14];
	result.m[14] = temp;

	return result;
}

Matrix4 Matrix4::operator*(const Matrix4 & other)
{
	Matrix4 matrix;

	matrix.m[0] = m[0] * other.m[0] + m[1] * other.m[4] + m[2] * other.m[8] + m[3] * other.m[12];
	matrix.m[1] = m[0] * other.m[1] + m[1] * other.m[5] + m[2] * other.m[9] + m[3] * other.m[13];
	matrix.m[2] = m[0] * other.m[2] + m[1] * other.m[6] + m[2] * other.m[10] + m[3] * other.m[14];
	matrix.m[3] = m[0] * other.m[3] + m[1] * other.m[7] + m[2] * other.m[11] + m[3] * other.m[15];

	matrix.m[4] = m[4] * other.m[0] + m[5] * other.m[4] + m[6] * other.m[8] + m[7] * other.m[12];
	matrix.m[5] = m[4] * other.m[1] + m[5] * other.m[5] + m[6] * other.m[9] + m[7] * other.m[13];
	matrix.m[6] = m[4] * other.m[2] + m[5] * other.m[6] + m[6] * other.m[10] + m[7] * other.m[14];
	matrix.m[7] = m[4] * other.m[3] + m[5] * other.m[7] + m[6] * other.m[11] + m[7] * other.m[15];

	matrix.m[8] = m[8] * other.m[0] + m[9] * other.m[4] + m[10] * other.m[8] + m[11] * other.m[12];
	matrix.m[9] = m[8] * other.m[1] + m[9] * other.m[5] + m[10] * other.m[9] + m[11] * other.m[13];
	matrix.m[10] = m[8] * other.m[2] + m[9] * other.m[6] + m[10] * other.m[10] + m[11] * other.m[14];
	matrix.m[11] = m[8] * other.m[3] + m[9] * other.m[7] + m[10] * other.m[11] + m[11] * other.m[15];

	matrix.m[12] = m[12] * other.m[0] + m[13] * other.m[4] + m[14] * other.m[8] + m[15] * other.m[12];
	matrix.m[13] = m[12] * other.m[1] + m[13] * other.m[5] + m[14] * other.m[9] + m[15] * other.m[13];
	matrix.m[14] = m[12] * other.m[2] + m[13] * other.m[6] + m[14] * other.m[10] + m[15] * other.m[14];
	matrix.m[15] = m[12] * other.m[3] + m[13] * other.m[7] + m[14] * other.m[11] + m[15] * other.m[15];

	return matrix;
}

Matrix4 Matrix4::Projection(float fov, float aspect, float n, float f)
{
	Matrix4 projection;

	/*float temp = tanf(fov / 2 * Math::PI_F / 180.0f);

	projection.md[0][0] = temp;
	projection.md[1][1] = temp;
	projection.md[2][2] = -((f) / (f - n));
	projection.md[2][3] = -1.0f;
	projection.md[3][2] = -((f * n) / (f - n));
	projection.md[3][3] = 0;*/

	float temp = Math::cotan(fov / 2);

	projection.m[0] = temp / aspect;
	projection.m[5] = temp;
	projection.m[10] = (f + n) / (f - n);
	projection.m[11] = 1.0f;
	projection.m[14] = -((f * n) / (f - n));
	projection.m[15] = 0;

	return projection;
}

Matrix4 Matrix4::LookAt(Vector4 & eyePosition, Vector4 & eyeDirection, Vector4 & upVector)
{
	Vector4 forward = Vector4::Normalize(eyePosition - eyeDirection);
	Vector4 right = Vector4::Cross(upVector, forward);
	Vector4 up = Vector4::Cross(forward, right);

	Matrix4 camToWorld;

	camToWorld.md[0][0] = right.x;
	camToWorld.md[0][1] = right.y;
	camToWorld.md[0][2] = right.z;
	camToWorld.md[0][3] = 0;

	camToWorld.md[1][0] = up.x;
	camToWorld.md[1][1] = up.y;
	camToWorld.md[1][2] = up.z;
	camToWorld.md[1][3] = 0;

	camToWorld.md[2][0] = forward.x;
	camToWorld.md[2][1] = forward.y;
	camToWorld.md[2][2] = forward.z;
	camToWorld.md[2][3] = 0;

	camToWorld.md[3][0] = eyePosition.x;
	camToWorld.md[3][1] = eyePosition.y;
	camToWorld.md[3][2] = eyePosition.z;
	camToWorld.md[3][3] = 1.0f;

	Matrix4 negatePosition = Identity();
	camToWorld.md[0][3] = -eyePosition.x;
	camToWorld.md[1][3] = -eyePosition.x;
	camToWorld.md[2][3] = -eyePosition.x;

	Matrix4 res = camToWorld * negatePosition;
	res.Transpose();

	return res;
}

Matrix4 Matrix4::LookAtDx(Vector4 & eyePosition, Vector4 & eyeDirection, Vector4 & upVector)
{
	Vector4 forward = Vector4::Normalize(eyePosition - eyeDirection);
	Vector4 right = Vector4::Cross(upVector, forward);
	Vector4 up = Vector4::Cross(forward, right);
	Vector4 negatePosition = Vector4::Negatate(eyePosition);
	
	
	Vector4 D0 = Vector4::Dot(right, negatePosition);
	Vector4 D1 = Vector4::Dot(up, negatePosition);
	Vector4 D2 = Vector4::Dot(forward, negatePosition);

	Matrix4 M;
	Vector4 select(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000);
	M.v.v1 = Vector4::Select(D0, right, select);
	M.v.v2 = Vector4::Select(D1, up, select);
	M.v.v3 = Vector4::Select(D2, forward, select);
	M.v.v4 = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

	M = M.Transpose();

	return M;
}