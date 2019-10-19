#pragma once

#ifndef HW_VERTEX_H
#define HW_VERTEX_H

#include "Hollow/Math/Vector2.h"
#include "Hollow/Math/Vector3.h"

#define NUM_BONES_PER_VEREX 4

namespace Hollow {
	struct VertexBoneData
	{
		int joints[NUM_BONES_PER_VEREX] {};
		float weights[NUM_BONES_PER_VEREX] {};
	};

	struct Vertex
	{
	public:
		Vertex() = default;

		Vertex(float px, float py, float pz, float tv, float tu, float nx, float ny, float nz)
			: pos(px, py, pz), texCoord(tv, tu), normal(nx, ny, nz), tangent(0.0f, 0.0f, 0.0f), bitangent(0.0f, 0.0f, 0.0f)
		{}

		Vertex(float px, float py, float pz) :
			Vertex(px, py, pz, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)
		{}

		Vertex(float px, float py, float pz, float tv, float tu)
			: Vertex(px, py, pz, tv, tu, 0.0f, 0.0f, 0.0f)
		{}
	public:
		Vector3 pos;
		Vector2 texCoord;
		Vector3 normal;
		Vector3 tangent;
		Vector3 bitangent;
		VertexBoneData boneData;
	};
}

#endif