#pragma once

#include "Hollow/ECS/Component.h"
#include "Hollow/Platform.h"
#include "Hollow/Import/Mesh.h"

struct Node 
{
	int id;
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
	double duration = 0.0;
};

class AnimationComponent : public Hollow::Component<AnimationComponent>
{
public:
	std::unordered_map<int, Node*> nodes;
	std::vector<Animation*> animations;
	Node* rootNode;
	double currentAnimationTime = 0.0;
	int currentAnimation = 0;
	Hollow::Matrix4* nodeInfo;
	int numNodes;
	double frameRate = 1.0 / 60.0;
	double currentFrame = 1.0 / 30.0;
public:
	AnimationComponent(const Hollow::s_ptr<Hollow::Import::Model>& mesh)
	{
		rootNode = new Node();
		rootNode->id = mesh->rootNode->id;
		rootNode->name = mesh->rootNode->name;
		rootNode->localTransform = mesh->rootNode->localTransform;

		parseNodes(rootNode, mesh->rootNode);

		for (auto& importAnimation : mesh->animations) {
			Animation* animation = new Animation();
			animation->duration = importAnimation->duration;

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

		numNodes = nodes.size();
		nodeInfo = new Hollow::Matrix4[200];
	}
private:
	void parseNodes(Node* parentNode, Hollow::Import::AnimationNode* node)
	{
		for (auto& it : node->childrens) {
			Node* childNode = new Node();
			childNode->id = it->id;
			childNode->localTransform = it->localTransform;
			childNode->name = it->name;

			parentNode->childs.push_back(childNode);
			nodes[childNode->id] = childNode;

			parseNodes(childNode, it);
		}
	}
};