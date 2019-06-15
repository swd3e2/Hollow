#pragma once

#include "ECS/System.h"
#include "ECS/EntityManager.h"
#include "Hollow/ECS/AnimationComponent.h"
#include <assimp/scene.h>
#include "Hollow/ECS/GLTFAnimation.h"
#include "Hollow/ECS/GameObject.h"

namespace Hollow {
	class AnimationSystem : public System<AnimationSystem>
	{
	public:
		virtual void PreUpdate(double dt) override {}

		virtual void Update(double dt) override
		{
			for (auto& entity : EntityManager::instance()->getContainer<GameObject>()->entityList)
			{
				if (entity.hasComponent<GLTFAnimation>())
				{
					GLTFAnimation* animationComponent = entity.getComponent<GLTFAnimation>();
					animationComponent->tick(dt);
					animate(animationComponent->time, animationComponent->rootNode, Matrix4::Identity(), animationComponent);
				}
			}
		}

		virtual void PostUpdate(double dt) override {}

		void animate(double time, Hollow::AnimationNode* bone, const Matrix4& ParentTransform, GLTFAnimation* component)
		{
			Matrix4 NodeTransformation(bone->localTransform);

			Hollow::NodeKeyFrameData* translationFrame = component->animations[0].findKeyFrameTranslation(time, bone->gltfId);
			Hollow::NodeKeyFrameData* translationNextFrame = component->animations[0].findKeyNextFrameTranslation(time, bone->gltfId);

			Hollow::NodeKeyFrameData* scaleFrame = component->animations[0].findKeyFrameScale(time, bone->gltfId);
			Hollow::NodeKeyFrameData* scaleNextFrame = component->animations[0].findKeyNextFrameScale(time, bone->gltfId);

			Hollow::NodeKeyFrameData* rotationFrame = component->animations[0].findKeyFrameRotation(time, bone->gltfId);
			Hollow::NodeKeyFrameData* rotationNextFrame = component->animations[0].findKeyNextFrameRotation(time, bone->gltfId);

			Matrix4 Scaling = Matrix4::Identity();
			Matrix4 Rotation = Matrix4::Identity();
			Matrix4 Translation = Matrix4::Identity();

			if (scaleFrame && scaleNextFrame) {
				Scaling = CalcInterpolatedScaling(scaleFrame, scaleNextFrame, time);
			}
			if (rotationFrame && rotationNextFrame) {
				Rotation = CalcInterpolatedRotation(rotationFrame, rotationNextFrame, time);
			}
			if (translationFrame && translationNextFrame) {
				Translation = CalcInterpolatedPosition(translationFrame, translationNextFrame, time);
			}

			NodeTransformation = Matrix4::Transpose(Rotation * Translation);

			Matrix4 GlobalTransformation = ParentTransform * NodeTransformation;

			component->boneInfo[bone->id] = component->globalInverse * GlobalTransformation * bone->localTransform;

			for (auto& it : bone->childrens) {
				animate(time, it, GlobalTransformation, component);
			}
		}

		Matrix4 CalcInterpolatedScaling(Hollow::NodeKeyFrameData* frame, Hollow::NodeKeyFrameData* nextFrame, double time)
		{
			float DeltaTime = (float)(nextFrame->time - frame->time);
			float Factor = (time - (float)frame->time) / DeltaTime;
			assert(Factor >= 0.0f && Factor <= 1.0f);
			Vector4 Start = frame->scale;
			Vector4 End = nextFrame->scale;
			Vector4 Delta = End - Start;
			Vector4 Out = Start + (Delta * Factor);

			return Matrix4::Scaling(Out);
		}

		Matrix4 CalcInterpolatedRotation(Hollow::NodeKeyFrameData* frame, Hollow::NodeKeyFrameData* nextFrame, double time)
		{
			float DeltaTime = (float)(nextFrame->time - frame->time);
			float Factor = (time - (float)frame->time) / DeltaTime;
			assert(Factor >= 0.0f && Factor <= 1.0f);
			Quaternion out;
			Quaternion::Interpolate(out, frame->rotation, nextFrame->rotation, Factor);
			out = out.Normalize();

			return out.toMatrix4();
		}

		Matrix4 CalcInterpolatedPosition(Hollow::NodeKeyFrameData* frame, Hollow::NodeKeyFrameData* nextFrame, double time)
		{
			float DeltaTime = (float)(nextFrame->time - frame->time);
			float Factor = (time - (float)frame->time) / DeltaTime;
			assert(Factor >= 0.0f && Factor <= 1.0f);
			Vector4 Start = frame->translation;
			Vector4 End = nextFrame->translation;
			Vector4 Delta = End - Start;
			Vector4 Out = Start + (Delta * Factor);

			return Matrix4::Translation(Out);
		}
	};
}
