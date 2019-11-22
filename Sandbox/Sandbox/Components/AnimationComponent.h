#pragma once

#include "Hollow/ECS/Component.h"
#include "Hollow/Platform.h"
#include "Hollow/Import/Mesh.h"

struct Joint
{
	int id;
	int jointId;
	std::string name;
	std::vector<Hollow::s_ptr<Joint>> childs;
	Hollow::s_ptr<Joint> parent;
	Hollow::Matrix4 inverseBindMatrix;
	Hollow::Matrix4 localTransform;
	Hollow::Quaternion rotation;
	Hollow::Vector3 translation;
	Hollow::Vector3 scale;

	Hollow::Quaternion currentRotation;
	Hollow::Vector3 currentTranslation;
	Hollow::Vector3 currentScale;

	double currentRotationIndex = 0;
	double currentTranslationIndex = 0;
	double currentScaleIndex = 0;
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

class AnimationComponent : public Hollow::Component<AnimationComponent>
{
public:
	float blendingFactor = 1.0f;
	/* List of nodes */
	std::unordered_map<int, Hollow::s_ptr<Joint>> nodes;
	/* List of animations */
	std::vector<Hollow::s_ptr<Animation>> animations;
	/* Root animation node */
	Hollow::s_ptr<Joint> rootJoint;
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
		if (mesh == nullptr || mesh->rootNode == nullptr) {
			return;
		}

		rootJoint = std::make_shared<Joint>();
		rootJoint->id = mesh->rootJoint->id;
		rootJoint->jointId = mesh->rootJoint->jointId;
		rootJoint->name = mesh->rootJoint->name;
		rootJoint->localTransform = mesh->rootJoint->localTransform;
		rootJoint->inverseBindMatrix = mesh->rootJoint->inverseBindMatrix;

		rootJoint->rotation = mesh->rootJoint->rotation;
		rootJoint->translation = mesh->rootJoint->translation;
		rootJoint->scale = mesh->rootJoint->scale;

		parseNodes(rootJoint, mesh->rootJoint.get());

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

		nodeInfo.resize(nodes.size() + 1);
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
		for (auto& it : nodes) {
			it.second->currentRotationIndex = it.second->currentScaleIndex = it.second->currentTranslationIndex = 0;
		}
	}
private:
	void parseNodes(const Hollow::s_ptr<Joint>& parentNode, Hollow::Import::Joint* joint)
	{
		for (auto& it : joint->childs) {
			Hollow::s_ptr<Joint> childNode = std::make_shared<Joint>();
			childNode->id = it->id;
			childNode->jointId = it->jointId;
			childNode->localTransform = it->localTransform;
			childNode->inverseBindMatrix = it->inverseBindMatrix;
			childNode->name = it->name;

			childNode->rotation = it->rotation;
			childNode->translation = it->translation;
			childNode->scale = it->scale;

			childNode->parent = parentNode;

			parentNode->childs.push_back(childNode);
			nodes[childNode->id] = childNode;

			parseNodes(childNode, it);
		}
	}
};