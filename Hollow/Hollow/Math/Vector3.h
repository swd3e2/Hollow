#pragma once

#ifndef HW_VECTOR_3_H
#define HW_VECTOR_3_H

#include "Hollow/Platform.h"

class HOLLOW_API Vector3
{
public:
	float x;
	float y;
	float z;
public:
	Vector3();
	Vector3(float x, float y, float z);

	Vector3(const Vector3& vec);
	Vector3(Vector3&& vec);

	Vector3& operator=(const Vector3& vec);
	Vector3& operator=(Vector3&& vec);
};

#endif