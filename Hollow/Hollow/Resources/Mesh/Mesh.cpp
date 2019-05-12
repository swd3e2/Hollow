#include "Mesh.h"

void Mesh::animate(double time, Bone* bone, const Matrix4& ParentTransform)
{
	if (boneInfo == nullptr)
	{
		boneInfo = new Matrix4[bones.size()];
	}

	Matrix4 NodeTransformation(bone->localTransform);
	KeyFrame* frame = animations[0]->findKeyFrame(0.0f, bone->name);
	KeyFrame* nextFrame = animations[0]->findKeyNextFrame(0.0f, bone->name);

	//if (frame)
	//{
	//	/*Matrix4 Scaling = CalcInterpolatedScaling(frame, nextFrame, time);
	//	Matrix4 Rotation = CalcInterpolatedRotation(frame, nextFrame, time);
	//	Matrix4 Translation = Matrix4::Translation(frame->transform.translate.x, frame->transform.translate.y, frame->transform.translate.z);*/
	//	NodeTransformation = Matrix4::Translation(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
	//}

	Matrix4 GlobalTransformation = ParentTransform * NodeTransformation;

	int BoneIndex = bone->id;

	boneInfo[BoneIndex] = m_GlobalInverseTransform * GlobalTransformation * bone->localTransform;
	for (auto& it : bone->childs)
	{
		animate(time, it, GlobalTransformation);
	}
}

Matrix4 Mesh::CalcInterpolatedScaling(KeyFrame* frame, KeyFrame* nextFrame, double time)
{
	float DeltaTime = (float)(frame->time - nextFrame->time);
	float Factor = (animations[0]->duration - (float)frame->time) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	Vector4 Start = frame->transform.scale;
	Vector4 End = nextFrame->transform.scale;
	Vector4 Delta = End - Start;
	Vector4 Out = Start + Delta * Factor;
	return Matrix4::Scaling(Out);
}

Matrix4 Mesh::CalcInterpolatedRotation(KeyFrame* frame, KeyFrame* nextFrame, double time)
{
	float DeltaTime = (float)(nextFrame->time - frame->time);
	float Factor = (animations[0]->duration - (float)frame->time) / DeltaTime;

	/*const aiQuaternion & StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion & EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();*/
	return Matrix4();
}