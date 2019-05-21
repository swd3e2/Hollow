#pragma once

#include "Hollow/Math/Vector4.h"
#include "Hollow/Math/Quaternion.h"

class  BoneTransform
{
public:
	Vector4 translate;
	Vector4 scale;
	Quaternion rotation;
public:
	BoneTransform() {}
};