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
		KeyFrame* frame = nullptr;
		if (keyFrames.find(bone) != keyFrames.end())
		{
			for (auto& it : keyFrames[bone])
			{
				if (it->time <= time) {
					frame = it;
				}
			}
			if (frame == nullptr && keyFrames[bone].size()) {
				keyFrames[bone][keyFrames[bone].size() - 1];
			}
		}
		
		return frame;
	}

	inline KeyFrame* findKeyNextFrame(float time, std::string& bone)
	{
		KeyFrame* frame = nullptr;

		if (keyFrames.find(bone) != keyFrames.end())
		{
			for (auto& it : keyFrames[bone])
			{
				if (it->time > time) {
					frame = it;
					break;
				}
			}
			if (frame == nullptr && keyFrames[bone].size()) {
				keyFrames[bone][keyFrames[bone].size() - 1];
			}
		}
		
		return frame;
	}
};