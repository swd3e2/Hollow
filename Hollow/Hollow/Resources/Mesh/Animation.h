#pragma once

#include <vector>
#include "KeyFrame.h"
#include <unordered_map>
#include "Hollow/Math/Matrix4.h"
#include "Hollow/Platform.h"
#include <string>

class HOLLOW_API Animation
{
public:
	std::unordered_map<std::string, std::vector<KeyFrame*>> keyFrames;
	double duration;
public:
	inline KeyFrame* findKeyFrame(float time, std::string& bone)
	{
		if (keyFrames.find(bone) != keyFrames.end())
		{
			for (auto& it : keyFrames[bone])
			{
				if (it->time < time) {
					return it;
				}
			}
			return keyFrames[bone][keyFrames[bone].size() - 1];
		}
		return nullptr;
	}

	inline KeyFrame* findKeyNextFrame(float time, std::string& bone)
	{
		if (keyFrames.find(bone) != keyFrames.end())
		{
			for (auto& it : keyFrames[bone])
			{
				if (it->time >= time) {
					return it;
				}
			}
			return keyFrames[bone][keyFrames[bone].size() - 1];
		}
		return nullptr;
	}
};