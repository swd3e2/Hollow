#pragma once

#include <string>
#include "Hollow/Math/Matrix4.h"
#include <vector>

class HOLLOW_API Bone
{
public:
	int id;
	std::string name;
	Matrix4 localTransform;
	std::vector<Bone*> childs;
	Bone* parent;
};