#include "Matrix4.h"

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

Matrix4 Matrix4::Rotation(const Vector4& vec)
{
	Matrix4 M = RotationX(vec.x);
	Matrix4 M1 = RotationY(vec.y);
	Matrix4 M2 = RotationZ(vec.z);

	return M * M1* M2;
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

Matrix4 Matrix4::operator*(const Matrix4 & other) const
{
	Matrix4 matrix;

	matrix.md[0][0] = (md[0][0] * other.md[0][0]) + (md[0][1] * other.md[1][0]) + (md[0][2] * other.md[2][0]) + (md[0][3] * other.md[3][0]);
	matrix.md[0][1] = (md[0][0] * other.md[0][1]) + (md[0][1] * other.md[1][1]) + (md[0][2] * other.md[2][1]) + (md[0][3] * other.md[3][1]);
	matrix.md[0][2] = (md[0][0] * other.md[0][2]) + (md[0][1] * other.md[1][2]) + (md[0][2] * other.md[2][2]) + (md[0][3] * other.md[3][2]);
	matrix.md[0][3] = (md[0][0] * other.md[0][3]) + (md[0][1] * other.md[1][3]) + (md[0][2] * other.md[2][3]) + (md[0][3] * other.md[3][3]);

	matrix.md[1][0] = (md[1][0] * other.md[0][0]) + (md[1][1] * other.md[1][0]) + (md[1][2] * other.md[2][0]) + (md[1][3] * other.md[3][0]);
	matrix.md[1][1] = (md[1][0] * other.md[0][1]) + (md[1][1] * other.md[1][1]) + (md[1][2] * other.md[2][1]) + (md[1][3] * other.md[3][1]);
	matrix.md[1][2] = (md[1][0] * other.md[0][2]) + (md[1][1] * other.md[1][2]) + (md[1][2] * other.md[2][2]) + (md[1][3] * other.md[3][2]);
	matrix.md[1][3] = (md[1][0] * other.md[0][3]) + (md[1][1] * other.md[1][3]) + (md[1][2] * other.md[2][3]) + (md[1][3] * other.md[3][3]);

	matrix.md[2][0] = (md[2][0] * other.md[0][0]) + (md[2][1] * other.md[1][0]) + (md[2][2] * other.md[2][0]) + (md[2][3] * other.md[3][0]);
	matrix.md[2][1] = (md[2][0] * other.md[0][1]) + (md[2][1] * other.md[1][1]) + (md[2][2] * other.md[2][1]) + (md[2][3] * other.md[3][1]);
	matrix.md[2][2] = (md[2][0] * other.md[0][2]) + (md[2][1] * other.md[1][2]) + (md[2][2] * other.md[2][2]) + (md[2][3] * other.md[3][2]);
	matrix.md[2][3] = (md[2][0] * other.md[0][3]) + (md[2][1] * other.md[1][3]) + (md[2][2] * other.md[2][3]) + (md[2][3] * other.md[3][3]);

	matrix.md[3][0] = (md[3][0] * other.md[0][0]) + (md[3][1] * other.md[1][0]) + (md[3][2] * other.md[2][0]) + (md[3][3] * other.md[3][0]);
	matrix.md[3][1] = (md[3][0] * other.md[0][1]) + (md[3][1] * other.md[1][1]) + (md[3][2] * other.md[2][1]) + (md[3][3] * other.md[3][1]);
	matrix.md[3][2] = (md[3][0] * other.md[0][2]) + (md[3][1] * other.md[1][2]) + (md[3][2] * other.md[2][2]) + (md[3][3] * other.md[3][2]);
	matrix.md[3][3] = (md[3][0] * other.md[0][3]) + (md[3][1] * other.md[1][3]) + (md[3][2] * other.md[2][3]) + (md[3][3] * other.md[3][3]);

	return matrix;
}

Matrix4 Matrix4::Projection(float fov, float aspect, float n, float f)
{
	Matrix4 projection;

	float temp = Math::cotan(fov / 2);

	projection.m[0] = temp / aspect;
	projection.m[5] = temp;
	projection.m[10] = (f + n) / (f - n);
	projection.m[11] = 1.0f;
	projection.m[14] = -((f * n) / (f - n));
	projection.m[15] = 0;

	return projection;
}

Matrix4 Matrix4::LookAt(const Vector4 & eyePosition, const Vector4 & eyeDirection, const Vector4 & upVector)
{
	Vector4 forward = Vector4::Normalize(eyePosition - eyeDirection);
	Vector4 right = Vector4::Cross(upVector, forward);
	Vector4 up = Vector4::Cross(forward, right);

	Matrix4 camToWorld;

	camToWorld.md[0][0] = right.x;
	camToWorld.md[1][0] = right.y;
	camToWorld.md[2][0] = right.z;
	camToWorld.md[3][0] = 0;

	camToWorld.md[0][1] = up.x;
	camToWorld.md[1][1] = up.y;
	camToWorld.md[2][1] = up.z;
	camToWorld.md[3][1] = 0;

	camToWorld.md[0][2] = forward.x;
	camToWorld.md[1][2] = forward.y;
	camToWorld.md[2][2] = forward.z;
	camToWorld.md[3][2] = 0;

	camToWorld.md[0][3] = -eyePosition.x;
	camToWorld.md[1][3] = -eyePosition.y;
	camToWorld.md[2][3] = -eyePosition.z;
	camToWorld.md[3][3] = 1.0f;

	return camToWorld;
}


void Matrix4::SetTranslation(const Vector4& vecPos)
{
	md[3][0] = vecPos.x;
	md[3][1] = vecPos.y;
	md[3][2] = vecPos.z;
}

Vector4 Matrix4::GetTranslation() const
{
	return Vector4(md[3][0], md[3][1], md[3][2], 0.0f);
}

Matrix4 Matrix4::InvertedTR() const
{
	Matrix4 M;

	// Create the transposed upper 3x3 matrix
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			M.md[i][j] = md[j][i];

	// The new matrix translation = -Rt
	M.SetTranslation(Vector4::Negatate(GetTranslation() * M));

	return M;
}

Vector4 operator*(const Vector4& vec, const Matrix4& mat)
{
	float x = vec.x * mat.md[0][0] + vec.y * mat.md[1][0] + vec.z * mat.md[2][0];
	float y = vec.x * mat.md[0][1] + vec.y * mat.md[1][1] + vec.z * mat.md[2][1];
	float z = vec.x * mat.md[0][2] + vec.y * mat.md[1][2] + vec.z * mat.md[2][2];

	return Vector4(x, y, z, vec.w);
}
