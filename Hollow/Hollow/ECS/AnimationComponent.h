#pragma once

#include "Component.h"
#include "Hollow/Importer/MeshImportData.h"

class AnimationComponent : public Component<AnimationComponent>
{
public:
	AnimationComponent(AnimationData* data) 
	{
		rootBone = data->rootBone;
		bones = data->bones;
		animations = data->animations;
		globalInverse = data->globalInverse;

		boneInfo = new Matrix4[100];
		time = 0.0f;
	}

	void tick(double time)
	{
		this->time += time * speed;
		if (this->time > animations[0]->duration) {
			this->time = 0;
		}
	}
public:
	Bone* rootBone;
	std::unordered_map<std::string, Bone*> bones;
	std::vector<Animation*> animations;
	Matrix4* boneInfo;
	Matrix4 globalInverse;

	double time;
	double speed = 0.01f;
};