#pragma once

#ifndef HW_VERTEX_H
#define HW_VERTEX_H

#include "Hollow/Math/Vector2.h"
#include "Hollow/Math/Vector3.h"

#define NUM_BONES_PER_VEREX 4

namespace Hollow {
	struct VertexBoneData
	{
		int IDs[NUM_BONES_PER_VEREX];
		float Weights[NUM_BONES_PER_VEREX];
	};

	struct Vertex
	{
		Vertex(float px, float py, float pz) :
			pos(px, py, pz)
		{
			for (int i = 0; i < NUM_BONES_PER_VEREX; i++)
			{
				boneData.IDs[i] = 0;
				boneData.Weights[i] = 0.0f;
			}
		}

		Vertex(float px, float py, float pz, float tv, float tu)
			: pos(px, py, pz), texCoord(tv, tu)
		{
			for (int i = 0; i < NUM_BONES_PER_VEREX; i++)
			{
				boneData.IDs[i] = 0;
				boneData.Weights[i] = 0.0f;
			}
		}

		Vertex() :
			pos(0.0f, 0.0f, 0.0f), texCoord(0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), tangent(0.0f, 0.0f, 0.0f), bitangent(0.0f, 0.0f, 0.0f)
		{
			for (int i = 0; i < NUM_BONES_PER_VEREX; i++)
			{
				boneData.IDs[i] = 0;
				boneData.Weights[i] = 0.0f;
			}
		}

		Vertex(float px, float py, float pz, float tv, float tu, float nx, float ny, float nz)
			: pos(px, py, pz), texCoord(tv, tu), normal(nx, ny, nz), tangent(0.0f, 0.0f, 0.0f), bitangent(0.0f, 0.0f, 0.0f)
		{
			for (int i = 0; i < NUM_BONES_PER_VEREX; i++)
			{
				boneData.IDs[i] = 0;
				boneData.Weights[i] = 0.0f;
			}
		}

		Vector3 pos;
		Vector2 texCoord;
		Vector3 normal;
		Vector3 tangent;
		Vector3 bitangent;
		VertexBoneData boneData;
	};
}

#endif