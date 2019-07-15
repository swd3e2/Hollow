#include "Test.h"

Hollow::Mesh* getCube()
{
	Hollow::Mesh* mesh = new Hollow::Mesh();
	Hollow::Model* model = new Hollow::Model();

	Hollow::Vertex cube_vertices[] = {
		// front
		Hollow::Vertex(-1.0, -1.0,  1.0, -1.0, -1.0,  1.0, -1.0, -1.0),
		Hollow::Vertex(1.0, -1.0,  1.0, -1.0, -1.0,  1.0, -1.0, -1.0),
		Hollow::Vertex(1.0,  1.0,  1.0, -1.0, -1.0,  1.0, -1.0, -1.0),
		Hollow::Vertex(-1.0,  1.0,  1.0, -1.0, -1.0,  1.0, -1.0, -1.0),
		// back
		Hollow::Vertex(-1.0, -1.0, -1.0, -1.0, -1.0,  1.0, -1.0, -1.0),
		Hollow::Vertex(1.0, -1.0, -1.0, -1.0, -1.0,  1.0, -1.0, -1.0),
		Hollow::Vertex(1.0,  1.0, -1.0, -1.0, -1.0,  1.0, -1.0, -1.0),
		Hollow::Vertex(-1.0,  1.0, -1.0, -1.0, -1.0,  1.0, -1.0, -1.0),
	};

	Hollow::VERTEX_BUFFER_DESC desc;
	desc.data = cube_vertices;
	desc.size = 8;
	desc.stride = sizeof(Hollow::Vertex);

	model->vBuffer = Hollow::HardwareBufferManager::instance()->create(desc);

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
	Hollow::INDEX_BUFFER_DESC idesc;
	desc.data = cube_elements;
	desc.size = 36;
	desc.stride = sizeof(unsigned int);

	model->iBuffer = Hollow::HardwareBufferManager::instance()->create(idesc);

	mesh->models.push_back(model);

	return mesh;
}