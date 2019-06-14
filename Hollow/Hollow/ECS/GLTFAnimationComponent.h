#pragma once
#include "Component.h"
#include "Hollow/Importer/gltf/GLTFImporter.h"
#include "Hollow/Math/Matrix4.h"

class GLTFAnimationComponent : public Component<GLTFAnimationComponent>
{
public:
	Hollow::AnimationNode* rootNode;
	std::vector<Hollow::Animation> animations;
	Matrix4* boneInfo;
	Matrix4 globalInverse;

	double time;
	double speed = 0.01f;
public:
	GLTFAnimationComponent(Hollow::GLTFModel* model)
	{
		animations = std::move(model->animations);
		rootNode = model->animationRootNode;
	}

	void tick(double time)
	{
		this->time += time * speed;
		if (this->time > 3.33) {
			this->time = 0;
		}
	}
};