#pragma once

#include "Resources/Mesh/Mesh.h"
#include "Graphics/Vertex.h"
#include "Graphics/HardwareBufferManager.h"

Mesh* getCube()
{
	Mesh* mesh = new Mesh();
	SubMesh* sMesh = new SubMesh();

	Vertex cube_vertices[] = {
		// front
		Vertex(-1.0, -1.0,  1.0, -1.0, -1.0,  1.0, -1.0, -1.0),
		Vertex(1.0, -1.0,  1.0, -1.0, -1.0,  1.0, -1.0, -1.0),
		Vertex(1.0,  1.0,  1.0, -1.0, -1.0,  1.0, -1.0, -1.0),
		Vertex(-1.0,  1.0,  1.0, -1.0, -1.0,  1.0, -1.0, -1.0),
		// back
		Vertex(-1.0, -1.0, -1.0, -1.0, -1.0,  1.0, -1.0, -1.0),
		Vertex(1.0, -1.0, -1.0, -1.0, -1.0,  1.0, -1.0, -1.0),
		Vertex(1.0,  1.0, -1.0, -1.0, -1.0,  1.0, -1.0, -1.0),
		Vertex(-1.0,  1.0, -1.0, -1.0, -1.0,  1.0, -1.0, -1.0),
	};

	sMesh->vBuffer = HardwareBufferManager::instance()->createVertexBuffer(cube_vertices, 8);

	unsigned int cube_elements[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};
	sMesh->iBuffer = HardwareBufferManager::instance()->createIndexBuffer(cube_elements, 36);
	mesh->subMeshes.push_back(sMesh);
	mesh->setReady(true);

	return mesh;
}