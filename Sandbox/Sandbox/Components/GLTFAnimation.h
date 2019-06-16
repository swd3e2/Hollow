#pragma once
#include <Hollow/ECS/Component.h>
#include "Hollow/Importer/gltf/GLTFImporter.h"

class GLTFAnimation : public Hollow::Component<GLTFAnimation>
{
public:
	Hollow::AnimationNode* rootNode;
	std::vector<Hollow::Animation> animations;
	Hollow::Matrix4* boneInfo;
	Hollow::Matrix4 globalInverse;

	double time;
	double speed = 0.01f;
public:
	GLTFAnimation(Hollow::GLTF::GLTFModel* model)
	{
		animations = std::move(model->animations);
		rootNode = model->rootAnimationNode;
		globalInverse = Hollow::Matrix4::Identity();

		boneInfo = new Hollow::Matrix4[100];
	}

	void tick(double time)
	{
		this->time += time * speed;
		if (this->time > animations[0].duration) {
			this->time = 0;
		}
	}
};