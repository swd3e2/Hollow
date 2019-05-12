#pragma once

#include "BoneTransform.h"
#include "Hollow/Platform.h"

class HOLLOW_API KeyFrame
{
public:
	int boneId;
	BoneTransform transform;
	double time;
	int index;
public:
	KeyFrame() {}
	~KeyFrame() {}
};