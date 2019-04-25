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

	matrix.md[0][0] = 1.0f;
	matrix.md[1][1] = 1.0f;
	matrix.md[2][2] = 1.0f;
	matrix.md[3][3] = 1.0f;

	return matrix;
}

Matrix4 Matrix4::Translation(float x, float y, float z)
{
	Matrix4 matrix = Identity();

	matrix.md[3][0] = x;
	matrix.md[3][1] = y;
	matrix.md[3][2] = z;

	return matrix;
}

Matrix4 Matrix4::Translation(const Vector4& vec)
{
	Matrix4 matrix = Identity();

	matrix.md[3][0] = vec.x;
	matrix.md[3][1] = vec.y;
	matrix.md[3][2] = vec.z;

	return matrix;
}

Matrix4 Matrix4::Scaling(float x, float y, float z)
{
	Matrix4 matrix;

	matrix.md[0][0] = x;
	matrix.md[1][1] = y;
	matrix.md[2][2] = z;
	matrix.md[3][3] = 1.0f;

	return matrix;
}

Matrix4 Matrix4::Scaling(const Vector4& vec)
{
	Matrix4 matrix;

	matrix.md[0][0] = vec.x;
	matrix.md[1][1] = vec.y;
	matrix.md[2][2] = vec.z;
	matrix.md[3][3] = 1.0f;

	return matrix;
}

Matrix4 Matrix4::Rotation(const Vector4& vec)
{
	Matrix4 M = RotationX(vec.x);
	Matrix4 M1 = RotationY(vec.y);
	Matrix4 M2 = RotationZ(vec.z);

	return M * M1 * M2;
}

Matrix4 Matrix4::Rotation(float x, float y, float z)
{
	Matrix4 M = RotationX(x);
	Matrix4 M1 = RotationY(y);
	Matrix4 M2 = RotationZ(z);

	return M  * M2 * M1;
}

Matrix4 Matrix4::RotationX(float x)
{
	Matrix4 matrix = Identity();

	matrix.md[1][1] = cosf(x);
	matrix.md[1][2] = -sinf(x);
	matrix.md[2][1] = sinf(x);
	matrix.md[2][2] = cosf(x);

	return matrix;
}

Matrix4 Matrix4::RotationY(float y)
{
	Matrix4 matrix = Identity();

	matrix.md[0][0] = cosf(y);
	matrix.md[0][2] = sinf(y);
	matrix.md[2][0] = -sinf(y);
	matrix.md[2][2] = cosf(y);

	return matrix;
}

Matrix4 Matrix4::RotationZ(float z)
{
	Matrix4 matrix = Identity();

	matrix.md[0][0] = cosf(z);
	matrix.md[0][1] = -sinf(z);
	matrix.md[1][0] = sinf(z);
	matrix.md[1][1] = cosf(z);

	return std::move(matrix);
}

Matrix4& Matrix4::Transpose()
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

Matrix4 Matrix4::Transpose(const Matrix4& matrix)
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

	projection.md[0][0] = temp / aspect;
	projection.md[1][1] = temp;
	projection.md[2][2] = (f + n) / (f - n);
	projection.md[2][3] = 1.0f;
	projection.md[3][2] = -((f * n) / (f - n));
	projection.md[3][3] = 0;

	return projection;
}

Matrix4 Matrix4::LookAt(const Vector4 & eyePosition, const Vector4 & eyeDirection, const Vector4 & upVector)
{
	Vector4 forward = Vector4::Normalize(eyeDirection - eyePosition);
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

	camToWorld.md[0][3] = -Vector4::Dot(right, eyePosition);
	camToWorld.md[1][3] = -Vector4::Dot(up, eyePosition);
	camToWorld.md[2][3] = -Vector4::Dot(forward, eyePosition);
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


Vector4 operator*(const Vector4& vec, const Matrix4& mat)
{
	float x = vec.x * mat.md[0][0] + vec.y * mat.md[1][0] + vec.z * mat.md[2][0];
	float y = vec.x * mat.md[0][1] + vec.y * mat.md[1][1] + vec.z * mat.md[2][1];
	float z = vec.x * mat.md[0][2] + vec.y * mat.md[1][2] + vec.z * mat.md[2][2];

	return Vector4(x, y, z, vec.w);
}
