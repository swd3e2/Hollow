#pragma once

#ifndef HW_VERTEX_H
#define HW_VERTEX_H

#include "Hollow/Math/Vector2.h"
#include "Hollow/Math/Vector3.h"

#define NUM_BONES_PER_VEREX 4

namespace Hollow {
	

	struct Vertex
	{
		Vertex(float px = 0.0f, float py = 0.0f, float pz = 0.0f, 
			float tv = 0.0f, float tu = 0.0f, 
			float nx = 0.0f, float ny = 0.0f, float nz = 0.0f)
			: pos(px, py, pz), 
			texCoord(tv, tu), 
			normal(nx, ny, nz), 
			tangent(0.0f, 0.0f, 0.0f), 
			bitangent(0.0f, 0.0f, 0.0f)
		{}

		Vector3 pos;
		Vector2 texCoord;
		Vector3 normal;
		Vector3 tangent;
		Vector3 bitangent;

		struct VertexBoneData
		{
			int joints[NUM_BONES_PER_VEREX]{};
			float weights[NUM_BONES_PER_VEREX]{};
		} boneData;
	};
}

#endif