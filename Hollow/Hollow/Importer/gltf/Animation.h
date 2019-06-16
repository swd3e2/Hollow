#pragma once

#ifndef HW_ANIMATION_H
#define HW_ANIMATION_H

#include "Hollow/Math/Matrix4.h"
#include "Hollow/Math/Quaternion.h"
#include <map>
#include <vector>

namespace Hollow {
	struct NodeKeyFrameData
	{
		Vector3 translation;
		Vector3 scale;
		Quaternion rotation;
		double time;
	};

	struct NodeAnimationData
	{
		int nodeId;
		std::map<float, NodeKeyFrameData> rotations;
		std::map<float, NodeKeyFrameData> translation;
		std::map<float, NodeKeyFrameData> scale;
	};

	struct AnimationNode
	{
		int gltfId;
		int id;
		Matrix4 localTransform;
		std::vector<AnimationNode*> childrens;
	};

	struct Animation
	{
		std::map<int, NodeAnimationData*> data;
		double duration = 0.0;

		inline NodeKeyFrameData* findKeyFrameTranslation(double time, int bone)
		{
			Hollow::NodeAnimationData* keyFrames = data[bone];
			NodeKeyFrameData* frame = nullptr;

			for (auto& it : keyFrames->translation)
			{
				if (it.first <= time) {
					frame = &it.second;
				}
			}
			if (frame == nullptr && keyFrames->translation.size()) {
				frame = &keyFrames->translation.rbegin()->second;
			}

			return frame;
		}

		inline NodeKeyFrameData* findKeyNextFrameTranslation(double time, int bone)
		{
			Hollow::NodeAnimationData* keyFrames = data[bone];
			NodeKeyFrameData* frame = nullptr;

			for (auto& it : keyFrames->translation)
			{
				if (it.first > time) {
					frame = &it.second;
				}
			}
			if (frame == nullptr && keyFrames->translation.size()) {
				frame = &keyFrames->translation.rbegin()->second;
			}

			return frame;
		}

		inline NodeKeyFrameData* findKeyFrameRotation(double time, int bone)
		{
			Hollow::NodeAnimationData* keyFrames = data[bone];
			NodeKeyFrameData* frame = nullptr;

			for (auto& it : keyFrames->rotations)
			{
				if (it.first <= time) {
					frame = &it.second;
				}
			}
			if (frame == nullptr && keyFrames->rotations.size()) {
				frame = &keyFrames->rotations.rbegin()->second;
			}

			return frame;
		}

		inline NodeKeyFrameData* findKeyNextFrameRotation(double time, int bone)
		{
			Hollow::NodeAnimationData* keyFrames = data[bone];
			NodeKeyFrameData* frame = nullptr;

			for (auto& it : keyFrames->rotations)
			{
				if (it.first > time) {
					frame = &it.second;
				}
			}
			if (frame == nullptr && keyFrames->rotations.size()) {
				frame = &keyFrames->rotations.rbegin()->second;
			}

			return frame;
		}

		inline NodeKeyFrameData* findKeyFrameScale(double time, int bone)
		{
			Hollow::NodeAnimationData* keyFrames = data[bone];
			NodeKeyFrameData* frame = nullptr;

			for (auto& it : keyFrames->scale)
			{
				if (it.first <= time) {
					frame = &it.second;
				}
			}
			if (frame == nullptr && keyFrames->scale.size()) {
				frame = &keyFrames->scale.rbegin()->second;
			}

			return frame;
		}

		inline NodeKeyFrameData* findKeyNextFrameScale(double time, int bone)
		{
			Hollow::NodeAnimationData* keyFrames = data[bone];
			NodeKeyFrameData* frame = nullptr;

			for (auto& it : keyFrames->scale)
			{
				if (it.first > time) {
					frame = &it.second;
				}
			}
			if (frame == nullptr && keyFrames->scale.size()) {
				frame = &keyFrames->scale.rbegin()->second;
			}

			return frame;
		}
	};
}

#endif