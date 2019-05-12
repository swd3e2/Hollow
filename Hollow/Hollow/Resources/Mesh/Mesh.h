#pragma once

#ifndef HW_MESH_H
#define HW_MESH_H

#include "SubMesh.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Math/Matrix4.h"
#include "Animation.h"
#include <assimp/scene.h>
#include <atomic>

class HOLLOW_API Mesh
{
private:
	std::atomic_bool ready;
public:
	int mNumSubmeshes;
	std::vector<SubMesh*> subMeshes;

	std::vector<Animation*> animations;
	std::unordered_map<std::string, Bone*> bones;

	Bone* rootBone;
	Matrix4 m_GlobalInverseTransform;
	Matrix4* boneInfo;

	std::string filename;
public:
	Mesh() : mNumSubmeshes(0), ready(false) {}

	inline void setReady(bool status) { ready = status; }
	inline bool getReady() { return ready; }
	void animate(double time, Bone* bone, const Matrix4& ParentTransform);
	inline bool hasAnimation() { return animations.size() > 0; }
	Matrix4 CalcInterpolatedScaling(KeyFrame* frame, KeyFrame* nextFrame, double time);
	Matrix4 CalcInterpolatedRotation(KeyFrame* frame, KeyFrame* nextFrame, double time);
	Matrix4 CalcInterpolatedPosition(KeyFrame* frame, KeyFrame* nextFrame, double time);
};

#endif