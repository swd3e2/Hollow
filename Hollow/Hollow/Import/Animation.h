#pragma once

#ifndef HW_ANIMATION_H
#define HW_ANIMATION_H

#include "Hollow/Math/Matrix4.h"
#include "Hollow/Math/Quaternion.h"
#include <map>
#include <vector>

namespace Hollow {
	namespace Import {
		struct AnimationNodeData
		{
			std::map<double, Quaternion> rotations;
			std::map<double, Vector3> positions;
			std::map<double, Vector3> scale;
		};

		struct AnimationNode
		{
			int id;
			Matrix4 localTransform;
			std::vector<AnimationNode*> childrens;
		};

		struct Animation
		{
			std::map<int, AnimationNodeData*> data;
			double duration = 0.0;
		};
	}
}
#endif