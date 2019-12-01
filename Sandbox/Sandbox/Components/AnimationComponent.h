#pragma once

#include "Hollow/ECS/Component.h"
#include "Hollow/Platform.h"
#include "Hollow/Import/Mesh.h"

class AnimationComponent : public Hollow::Component<AnimationComponent>
{
public:
	struct AnimationIndex 
	{
		double rotationIndex = 0;
		double translationIndex = 0;
		double scaleIndex = 0;
	};
	struct AnimationNodeData
	{
		std::vector<std::pair<double, Hollow::Quaternion>> rotations;
		std::vector<std::pair<double, Hollow::Vector3>> positions;
		std::vector<std::pair<double, Hollow::Vector3>> scale;
	};

	struct Animation
	{
		std::map<int, Hollow::s_ptr<AnimationNodeData>> data;
		std::string name;
		double duration = 0.0;
	};
public:
	std::vector<AnimationIndex> animationIndices;
	std::vector<Hollow::Matrix4> inverseBindMatrices;
	/* Joint ids in node list */
	std::vector<int> joints;
	/* Blending factor between two selected animations */
	float blendingFactor = 1.0f;
	/* List of animations */
	std::vector<Hollow::s_ptr<Animation>> animations;
	/* Root animation node */
	int rootJoint;
	/* Current animation time */
	double currentAnimationTime = 0.0;
	/* Selected animation index */
	int currentAnimation = 0;
	/* List of joint trs matrices*/
	std::vector<Hollow::Matrix4> nodeInfo;
	/* Animation frame rate*/
	double frameRate = 1.0 / 60.0;
	/* Current frame time */
	double currentFrame = 0.0;
	/*  */
	bool shouldPlay = true;
	/*  */
	bool stoped = false;
public:
	AnimationComponent(const Hollow::s_ptr<Hollow::Import::Model>& mesh)
	{
		if (mesh->rootJoint == -1) {
			return;
		}

		rootJoint = mesh->rootJoint;
		inverseBindMatrices = mesh->inverseBindMatrices;

		for (auto& importAnimation : mesh->animations) {
			Hollow::s_ptr<Animation> animation = std::make_shared<Animation>();
			animation->duration = importAnimation->duration;
			animation->name = importAnimation->name;

			for (auto& animationData : importAnimation->data) {
				Hollow::s_ptr<AnimationNodeData> nodeData = std::make_shared<AnimationNodeData>();

				for (auto& it : animationData.second->positions) {
					nodeData->positions.push_back(std::make_pair(it. first, it.second));
				}
				for (auto& it : animationData.second->rotations) {
					nodeData->rotations.push_back(std::make_pair(it. first, it.second));
				}
				for (auto& it : animationData.second->scale) {
					nodeData->scale.push_back(std::make_pair(it. first, it.second));
				}

				animation->data[animationData.first] = nodeData;
			}
			animations.push_back(animation);
		}

		nodeInfo.resize(mesh->joints.size());
		animationIndices.resize(mesh->joints.size());
	}

	void pause()
	{
		shouldPlay = false;
	}

	void restart()
	{
		stop();
		play();
	}

	void play()
	{
		shouldPlay = true;
		stoped = false;
	}

	void stop()
	{
		currentFrame = 0.0;
		currentAnimationTime = 0.0;
		resetAllNodeTRSIndices();
		shouldPlay = false;
		stoped = true;
	}

	void resetAllNodeTRSIndices()
	{
		for (auto& it : animationIndices) {
			it.rotationIndex = it.scaleIndex = it.translationIndex = 0;
		}
	}
};