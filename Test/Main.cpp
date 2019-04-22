#include <windows.h>
#include "array.h"
#include <cmath>
#include <iostream>
#include "Hollow/Math/Matrix3.h"
#include "Hollow/Math/Matrix4.h"
#include "Hollow/Math/Vector4.h"


int main() 
{
	Vector4 vec1(1.0f, 3.0f, -4.0f, 1.0f);
	Vector4 vec2(-1.0f, -12.0f, -2.0f, 1.0f);

	Vector4 vec3 = Vector4::Cross(vec1, vec2);
	//// Matrix3
	//Matrix3 matrix;
	//for (int i = 1; i < 10; i++)
	//	matrix.data[i-1] = i;

	//Matrix3 matrix2;
	//for (int i = 1; i < 10; i++)
	//	matrix2.data[i-1] = i;

	//Matrix3 matrix3 = matrix * matrix2;

	system("pause");
	return 0;
}

