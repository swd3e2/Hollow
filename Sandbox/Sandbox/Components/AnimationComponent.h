#pragma once

#include "Hollow/ECS/Component.h"
#include "Hollow/Platform.h"
#include "Hollow/Import/Mesh.h"

struct Node 
{
	int id;
	int jointId;
	std::string name;
	std::vector<Node*> childs;
	Hollow::Matrix4 localTransform;
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
	std::map<int, AnimationNodeData*> data;
	std::string name;
	double duration = 0.0;
};

class AnimationComponent : public Hollow::Component<AnimationComponent>
{
public:
	/* List of nodes */
	std::unordered_map<int, Node*> nodes;
	/* List of animations */
	std::vector<Animation*> animations;
	/* Root animation node */
	Node* rootNode;
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
		rootNode = new Node();
		rootNode->id = mesh->rootNode->id;
		rootNode->jointId = mesh->rootNode->jointId;
		rootNode->name = mesh->rootNode->name;
		rootNode->localTransform = mesh->rootNode->localTransform;

		parseNodes(rootNode, mesh->rootNode);

		for (auto& importAnimation : mesh->animations) {
			Animation* animation = new Animation();
			animation->duration = importAnimation->duration;
			animation->name = importAnimation->name;

			for (auto& animationData : importAnimation->data) {
				AnimationNodeData* nodeData = new AnimationNodeData();

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
	void parseNodes(Node* parentNode, Hollow::Import::AnimationNode* node)
	{
		for (auto& it : node->childrens) {
			Node* childNode = new Node();
			childNode->id = it->id;
			childNode->jointId = it->jointId;
			childNode->localTransform = it->localTransform;
			childNode->name = it->name;

			parentNode->childs.push_back(childNode);
			nodes[childNode->id] = childNode;

			parseNodes(childNode, it);
		}
	}
};