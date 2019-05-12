#include "Mesh.h"

void Mesh::animate(double time, Bone* bone, const Matrix4& ParentTransform)
{
	if (boneInfo == nullptr)
	{
		boneInfo = new Matrix4[100];
	}

	Matrix4 NodeTransformation(bone->localTransform);
	KeyFrame* frame = animations[0]->findKeyFrame(time, bone->name);
	KeyFrame* nextFrame = animations[0]->findKeyNextFrame(time, bone->name);

	if (frame && nextFrame)
	{
		Matrix4 Scaling = CalcInterpolatedScaling(frame, nextFrame, time);
		Matrix4 Rotation = CalcInterpolatedRotation(frame, nextFrame, time);
		Matrix4 Translation = CalcInterpolatedPosition(frame, nextFrame, time);
		NodeTransformation = Translation * Rotation;
	}

	Matrix4 GlobalTransformation = NodeTransformation * ParentTransform;
	boneInfo[bone->id] =  Matrix4::Transpose(bone->localTransform * GlobalTransformation) * m_GlobalInverseTransform;

	for (auto& it : bone->childs)
	{
		animate(time, it, GlobalTransformation);
	}
}

Matrix4 Mesh::CalcInterpolatedScaling(KeyFrame* frame, KeyFrame* nextFrame, double time)
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

Matrix4 Mesh::CalcInterpolatedRotation(KeyFrame* frame, KeyFrame* nextFrame, double time)
{
	float DeltaTime = (float)(nextFrame->time - frame->time);
	float Factor = (time - (float)frame->time) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	aiQuaternion StartRotationQ(frame->transform.rotation.x, frame->transform.rotation.y, frame->transform.rotation.z, 0.0f);
	aiQuaternion EndRotationQ(nextFrame->transform.rotation.x, nextFrame->transform.rotation.y, nextFrame->transform.rotation.z, 0.0f);
	aiQuaternion out;
	aiQuaternion::Interpolate(out, StartRotationQ, EndRotationQ, Factor);
	out = out.Normalize();

	return Matrix4((const float*)&out.GetMatrix(), 9);
}

Matrix4 Mesh::CalcInterpolatedPosition(KeyFrame* frame, KeyFrame* nextFrame, double time)
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