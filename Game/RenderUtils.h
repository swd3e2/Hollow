#pragma once

#include <Hollow/Math/Matrix4.h>
#include <Hollow/Math/Vector2.h>

struct Vertex
{
	Hollow::Vector3 position;
	Hollow::Vector2 texCoord;

	Vertex(Hollow::Vector3 position, Hollow::Vector2 texCoord) : 
		position(position), texCoord(texCoord)
	{}
};

struct WorldViewProjectionData
{
	Hollow::Matrix4 ViewMatrix;
	Hollow::Matrix4 ProjectionMatrix;
};

struct RenderData
{
	Hollow::Matrix4 transformMatrix;
};