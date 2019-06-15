#pragma once
#include "Component.h"
#include "Hollow/Importer/gltf/GLTFImporter.h"

class GLTFAnimation : public Component<GLTFAnimation>
{
public:
	Hollow::AnimationNode* rootNode;
	std::vector<Hollow::Animation> animations;
	Matrix4* boneInfo;
	Matrix4 globalInverse;

	double time;
	double speed = 0.01f;
public:
	GLTFAnimation(Hollow::GLTFModel* model)
	{
		animations = std::move(model->animations);
		rootNode = model->rootAnimationNode;
		globalInverse = Matrix4::Identity();

		boneInfo = new Matrix4[100];
	}

	void tick(double time)
	{
		this->time += time * speed;
		if (this->time > animations[0].duration) {
			this->time = 0;
		}
	}
};