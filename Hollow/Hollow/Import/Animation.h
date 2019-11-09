#pragma once

#ifndef HW_ANIMATION_H
#define HW_ANIMATION_H

#include "Hollow/Platform.h"
#include "Hollow/Math/Matrix4.h"
#include "Hollow/Math/Quaternion.h"
#include <map>
#include <vector>
#include <string>
#include "DirectXMath.h"
namespace Hollow {
	namespace Import {
		struct AnimationNodeData
		{
			DirectX::XMMATRIX m;
			std::map<double, Quaternion> rotations;
			std::map<double, Vector3> positions;
			std::map<double, Vector3> scale;
		};

		struct AnimationNode
		{
			int id;
			int jointId;
			std::string name;
			Matrix4 localTransform;
			Matrix4 localTransform2;
			Matrix4 inverseGlobalTransform;
			Quaternion rotation;
			Vector3 translation;
			Vector3 scale;
			std::vector<s_ptr<AnimationNode>> childrens;
		};

		struct Animation
		{
			std::map<int, s_ptr<AnimationNodeData>> data;
			double duration = 0.0;
			std::string name;
		};
	}
}
#endif