#pragma once

#include "Hollow/Common/Log.h"
#include <Hollow/ECS/System.h>
#include <Hollow/ECS/EntityManager.h>
#include "Sandbox/Components/AnimationComponent.h"
#include "Sandbox/Components/RenderableComponent.h"
#include "Sandbox/Entities/GameObject.h"
#include <tuple>
#include <utility>
#include "Sandbox/Profiler.h"

class AnimationSystem : public Hollow::System<AnimationSystem>
{
public:
	virtual void PreUpdate(double dt) override {}

	virtual void Update(double dt) override
	{
		Profiler::begin("Animation System: update()");
		for (auto& entity : Hollow::EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<AnimationComponent>() && entity.hasComponent<RenderableComponent>()) {
				AnimationComponent* animation = entity.getComponent<AnimationComponent>();
				RenderableComponent* renderable = entity.getComponent<RenderableComponent>();

				if (animation->shouldPlay) {
					animation->currentAnimationTime += dt / 1000.0;
				}

				if (animation->animations.size() > 0) {
					if (animation->currentAnimationTime > animation->animations[animation->currentAnimation]->duration) {
						animation->currentAnimationTime = 0.0;
						animation->resetAllNodeTRSIndices();
					}

					if (animation->currentFrame >= animation->frameRate) {
						animate(
							animation->currentAnimationTime,
							animation->rootJoint,
							animation->animations[animation->currentAnimation],
							animation->nodeInfo,
							animation,
							renderable
						);
						animation->currentFrame = 0.0f;
					}
					animation->currentFrame += dt / 1000.0;
				}
			}
		}
		Profiler::end();
	}

	virtual void PostUpdate(double dt) override {}

	void animate(double time, const int nodeId, const Hollow::s_ptr<AnimationComponent::Animation>& animation, std::vector<Hollow::Matrix4>& container, AnimationComponent* animationComponent, RenderableComponent* renderableComponent)
	{
		Hollow::Matrix4 localTransform = Hollow::Matrix4::identity();

		const Hollow::s_ptr<RenderableComponent::Node>& node = renderableComponent->nodes[nodeId];

		if (animation->data.find(node->id) != animation->data.end()) {
			const Hollow::s_ptr<AnimationComponent::AnimationNodeData>& data = animation->data[node->id];

			std::pair<double, Hollow::Vector3> closestScale;
			std::pair<double, Hollow::Vector3> closestTranslation;
			std::pair<double, Hollow::Quaternion> closestRotation;

			std::pair<double, Hollow::Vector3> nextClosestScale;
			std::pair<double, Hollow::Vector3> nextClosestTranslation;
			std::pair<double, Hollow::Quaternion> nextClosestRotation;

			std::tie(closestScale, closestTranslation, closestRotation, 
				nextClosestScale, nextClosestTranslation, nextClosestRotation) = getClosest(time, data, node, animationComponent);

			Hollow::Vector3 scaling = closestScale.first < nextClosestScale.first
				? interpolateScaling(closestScale, nextClosestScale, time)
				: closestScale.second;
			node->transform.scale = scaling;

			Hollow::Vector3 translation = closestTranslation.first < nextClosestTranslation.first
				? interpolatePosition(closestTranslation, nextClosestTranslation, time)
				: closestTranslation.second;
			node->transform.translation = translation;

			Hollow::Quaternion rotation = closestRotation.first < nextClosestRotation.first
				? interpolateRotation(closestRotation, nextClosestRotation, time)
				: closestRotation.second;
			node->transform.rotation = rotation;
			node->transform.hasChanged = true;

			// rotation = Hollow::Quaternion::interpolate(node->rotation, rotation, animationComponent->blendingFactor);
			// rotation.normalize();
			// translation = Hollow::Vector3::interpolate(node->translation, translation, animationComponent->blendingFactor);
		}

		if (node->jointId != -1) {
			container[node->jointId] = node->transform.worldTransform * animationComponent->inverseBindMatrices[node->jointId];
		}

		for (auto& it : node->childs) {
			animate(time, it, animation, container, animationComponent, renderableComponent);
		}
	}


	std::tuple<
		std::pair<double, Hollow::Vector3>, 
		std::pair<double, Hollow::Vector3>, 
		std::pair<double, Hollow::Quaternion>,
		std::pair<double, Hollow::Vector3>, 
		std::pair<double, Hollow::Vector3>, 
		std::pair<double, Hollow::Quaternion>
	> getClosest(const double time, const Hollow::s_ptr<AnimationComponent::AnimationNodeData>& data, const Hollow::s_ptr<RenderableComponent::Node>& node, AnimationComponent* animationComponent)
	{
		// Set all to zero so if we not found keyframe at 0 we could return at least something
		std::pair<double, Hollow::Vector3> scale;
		std::pair<double, Hollow::Vector3> nextScale;
		scale.second = Hollow::Vector3(1.0f, 1.0f, 1.0f);
		scale.first = 0.0f;
		if (data->scale.size() > 0) {
			scale = std::make_pair(data->scale.begin()->first, data->scale.begin()->second);
		} else {
			//translation = std::make_pair(0.0f, node->translation);
		}
		std::pair<double, Hollow::Vector3> translation;
		std::pair<double, Hollow::Vector3> nextTranslation;
		if (data->positions.size() > 0) {
			translation = std::make_pair(data->positions.begin()->first, data->positions.begin()->second);
		} else {
			translation = std::make_pair(0.0f, node->transform.translation);
		}
		std::pair<double, Hollow::Quaternion> rotation;
		std::pair<double, Hollow::Quaternion> nextRotation;
		if (data->rotations.size() > 0) {
			rotation = std::make_pair(data->rotations.begin()->first, data->rotations.begin()->second);
		} else {
			rotation = std::make_pair(0.0f, node->transform.rotation);
		}

		for (int i = animationComponent->animationIndices[node->jointId].scaleIndex; i < data->scale.size(); i++) {
			std::pair<double, Hollow::Vector3>& scale = data->scale[i];

			if (scale.first < time) {
				scale = std::make_pair(scale.first, scale.second);
				animationComponent->animationIndices[node->jointId].scaleIndex = i;
				if (i + 1 < data->scale.size()) {
					nextScale = std::make_pair(data->scale[i + 1].first, data->scale[i + 1].second);
				}
			} else {
				break;
			}
		}

		for (int i = animationComponent->animationIndices[node->jointId].translationIndex; i < data->positions.size(); i++) {
			std::pair<double, Hollow::Vector3>& position = data->positions[i];
			
			if (position.first < time) {
				translation = std::make_pair(position.first, position.second);
				animationComponent->animationIndices[node->jointId].translationIndex = i;
				if (i + 1 < data->positions.size()) {
					nextTranslation = std::make_pair(data->positions[i + 1].first, data->positions[i + 1].second);
				}
			} else {
				break;
			}
		}

		for (int i = animationComponent->animationIndices[node->jointId].rotationIndex; i < data->rotations.size(); i++) {
			std::pair<double, Hollow::Quaternion>& tRotation = data->rotations[i];

			if (tRotation.first < time) {
				rotation = std::make_pair(tRotation.first, tRotation.second);
				animationComponent->animationIndices[node->jointId].rotationIndex = i;
				if (i + 1 < data->rotations.size()) {
					nextRotation = std::make_pair(data->rotations[i + 1].first, data->rotations[i + 1].second);
				}
			} else {
				break;
			}
		}

		return std::make_tuple(scale, translation, rotation, nextScale, nextTranslation, nextRotation);
	}

	Hollow::Vector3 interpolatePosition(std::pair<double, Hollow::Vector3>& prev, std::pair<double, Hollow::Vector3>& next, double time)
	{
		if (next.first == 0.0 && prev.first == 0.0) {
			return Hollow::Vector3(0.0f, 0.0f, 0.0f);
		}

		float deltaTime = (float)(next.first - prev.first);
		float factor = (time - (float)prev.first) / deltaTime;

		return Hollow::Vector3::interpolate(prev.second, next.second, factor);
	}

	Hollow::Vector3 interpolateScaling(std::pair<double, Hollow::Vector3>& prev, std::pair<double, Hollow::Vector3>& next, double time)
	{
		if (next.first == 0.0 && prev.first == 0.0) {
			return Hollow::Vector3(1.0f, 1.0f, 1.0f);
		}

		float deltaTime = (float)(next.first - prev.first);
		float factor = (time - (float)prev.first) / deltaTime;

		return Hollow::Vector3::interpolate(prev.second, next.second, factor);
	}

	Hollow::Quaternion interpolateRotation(std::pair<double, Hollow::Quaternion>& prev, std::pair<double, Hollow::Quaternion>& next, double time)
	{
		if (next.first == 0.0 && prev.first == 0.0) {
			return Hollow::Quaternion();
		}

		float deltaTime = (float)(next.first - prev.first);
		float factor = (time - (float)prev.first) / deltaTime;

		Hollow::Quaternion out = Hollow::Quaternion::interpolate(prev.second, next.second, factor);
		out.normalize();

		return out;
	}
};
