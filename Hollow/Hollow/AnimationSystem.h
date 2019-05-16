#pragma once

#include "ECS/System.h"
#include "ECS/EntityManager.h"
#include "Hollow/ECS/AnimationComponent.h"
#include <assimp/scene.h>

namespace Hollow {
	class AnimationSystem : public System<AnimationSystem>
	{
	public:
		virtual void PreUpdate(float_t dt) override {}

		virtual void Update(float_t dt) override
		{
			for (auto& entity : EntityManager::instance()->getContainer<GameObject>()->entityList)
			{
				if (entity.hasComponent<AnimationComponent>())
				{
					AnimationComponent* animationComponent = entity.getComponent<AnimationComponent>();
					animationComponent->tick(dt);
					animate(animationComponent->time, animationComponent->rootBone, Matrix4::Identity(), animationComponent);
				}
			}
		}

		virtual void PostUpdate(float_t dt) override {}

		void animate(double time, Bone* bone, const Matrix4& ParentTransform, AnimationComponent* component)
		{
			Matrix4 NodeTransformation(bone->localTransform);
			KeyFrame* frame = component->animations[0]->findKeyFrame(time, bone->name);
			KeyFrame* nextFrame = component->animations[0]->findKeyNextFrame(time, bone->name);

			if (frame && nextFrame)
			{
				Matrix4 Scaling = CalcInterpolatedScaling(frame, nextFrame, time);
				Matrix4 Rotation = CalcInterpolatedRotation(frame, nextFrame, time);
				Matrix4 Translation = CalcInterpolatedPosition(frame, nextFrame, time);
				NodeTransformation = Matrix4::Transpose((Rotation)* Translation);
			}

			Matrix4 GlobalTransformation = ParentTransform * NodeTransformation;

			component->boneInfo[bone->id] = component->globalInverse * GlobalTransformation * bone->localTransform;

			for (auto& it : bone->childs)
			{
				animate(time, it, GlobalTransformation, component);
			}
		}

		Matrix4 CalcInterpolatedScaling(KeyFrame* frame, KeyFrame* nextFrame, double time)
		{
			float DeltaTime = (float)(nextFrame->time - frame->time);
			float Factor = (time - (float)frame->time) / DeltaTime;
			assert(Factor >= 0.0f && Factor <= 1.0f);
			Vector4 Start = frame->transform.scale;
			Vector4 End = nextFrame->transform.scale;
			Vector4 Delta = End - Start;
			Vector4 Out = Start + (Delta * Factor);

			return Matrix4::Scaling(Out);
		}

		Matrix4 CalcInterpolatedRotation(KeyFrame * frame, KeyFrame * nextFrame, double time)
		{
			float DeltaTime = (float)(nextFrame->time - frame->time);
			float Factor = (time - (float)frame->time) / DeltaTime;
			assert(Factor >= 0.0f && Factor <= 1.0f);
			aiQuaternion StartRotationQ(frame->transform.rotation.w, frame->transform.rotation.x, frame->transform.rotation.y, frame->transform.rotation.z);
			aiQuaternion EndRotationQ(nextFrame->transform.rotation.w, nextFrame->transform.rotation.x, nextFrame->transform.rotation.y, nextFrame->transform.rotation.z);
			aiQuaternion out;
			aiQuaternion::Interpolate(out, StartRotationQ, EndRotationQ, Factor);
			out = out.Normalize();

			return Matrix4((const float*)& out.GetMatrix().Transpose(), 9);
		}

		Matrix4 CalcInterpolatedPosition(KeyFrame * frame, KeyFrame * nextFrame, double time)
		{
			float DeltaTime = (float)(nextFrame->time - frame->time);
			float Factor = (time - (float)frame->time) / DeltaTime;
			assert(Factor >= 0.0f && Factor <= 1.0f);
			Vector4 Start = frame->transform.translate;
			Vector4 End = nextFrame->transform.translate;
			Vector4 Delta = End - Start;
			Vector4 Out = Start + (Delta * Factor);

			return Matrix4::Translation(Out);
		}
	};
}
