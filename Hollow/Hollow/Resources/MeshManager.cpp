#include "MeshManager.h"

void MeshManager::startUp()
{
	setStartedUp();
}

void MeshManager::shutdown()
{
	setShutdown();
}

Mesh* MeshManager::CreateMesh(std::string filename)
{
	MeshData* data = objParser.LoadObj(filename);
	Mesh* mesh = new Mesh(filename);

	if (data == nullptr) {
		return nullptr;
	}

	for (int i = 0; i < data->objects.size(); i++) {
		Containers::Vector<SimpleVertex>* vertices = new Containers::Vector<SimpleVertex>;

		for (int j = 0; j < data->objects[i]->indices.size(); j++) {

			SimpleVertex simpleVertex;
			simpleVertex.pos.x = data->vertices[data->objects[i]->indices[j].vertice_index * 3];
			simpleVertex.pos.y = data->vertices[1 + data->objects[i]->indices[j].vertice_index * 3];
			simpleVertex.pos.z = data->vertices[2 + data->objects[i]->indices[j].vertice_index * 3];

			if (data->objects[i]->has_texture) {
				simpleVertex.texCoord.x = -data->tex_coords[data->objects[i]->indices[j].tex_coord_index * 2];
				simpleVertex.texCoord.y = -data->tex_coords[1 + data->objects[i]->indices[j].tex_coord_index * 2];
			}
			else {
				simpleVertex.texCoord.x = 0.0f;
				simpleVertex.texCoord.y = 0.0f;
			}

			if (data->objects[i]->indices[j].normal_index > 0 && data->objects[i]->indices[j].normal_index * 3 + 2 < data->normals.size()) {
				simpleVertex.normal.x = data->normals[data->objects[i]->indices[j].normal_index * 3];
				simpleVertex.normal.y = data->normals[1 + data->objects[i]->indices[j].normal_index * 3];
				simpleVertex.normal.z = data->normals[2 + data->objects[i]->indices[j].normal_index * 3];
			}
			vertices->push_back(simpleVertex);
		}

		// tangents
		for (int j = 0; j < vertices->size(); j += 3)
		{
			SimpleVertex* firstVertex = &(*vertices)[j];
			SimpleVertex* secondVertex = &(*vertices)[j + 1];
			SimpleVertex* thirdVertex = &(*vertices)[j + 2];

			//Get the vector describing one edge of our triangle (edge 0,2)
			XMVECTOR edge1 = XMVectorSet(
				secondVertex->pos.x - firstVertex->pos.x,
				secondVertex->pos.y - firstVertex->pos.y,
				secondVertex->pos.z - firstVertex->pos.z,
				0.0f
			);    //Create our first edge

			//Get the vector describing another edge of our triangle (edge 2,1)
			XMVECTOR edge2 = XMVectorSet(
				thirdVertex->pos.x - firstVertex->pos.x,
				thirdVertex->pos.y - firstVertex->pos.y,
				thirdVertex->pos.z - firstVertex->pos.z,
				0.0f
			);    //Create our second edge

			//Find first texture coordinate edge 2d vector
			float deltaUV1x = secondVertex->texCoord.x - firstVertex->texCoord.x;
			float deltaUV1y = secondVertex->texCoord.y - firstVertex->texCoord.y;

			//Find second texture coordinate edge 2d vector
			float deltaUV2x = thirdVertex->texCoord.x - firstVertex->texCoord.x;
			float deltaUV2y = thirdVertex->texCoord.y - firstVertex->texCoord.y;

			//Find tangent using both tex coord edges and position edges
			XMFLOAT3 tangent = XMFLOAT3(0.0f, 0.0f, 0.0f);
			float r = 1.0f / (deltaUV1x * deltaUV2y - deltaUV2x * deltaUV1y);

			tangent.x = (deltaUV2y * XMVectorGetX(edge1) - deltaUV1y * XMVectorGetX(edge2)) * r;
			tangent.y = (deltaUV2y * XMVectorGetY(edge1) - deltaUV1y * XMVectorGetY(edge2)) * r;
			tangent.z = (deltaUV2y * XMVectorGetZ(edge1) - deltaUV1y * XMVectorGetZ(edge2)) * r;

			XMFLOAT3 bitangent = XMFLOAT3(0.0f, 0.0f, 0.0f);
			bitangent.x = (deltaUV1x * XMVectorGetX(edge2) - deltaUV2x * XMVectorGetX(edge1)) * r;
			bitangent.y = (deltaUV1x * XMVectorGetY(edge2) - deltaUV2x * XMVectorGetY(edge1)) * r;
			bitangent.z = (deltaUV1x * XMVectorGetZ(edge2) - deltaUV2x * XMVectorGetZ(edge1)) * r;

			firstVertex->tangent = tangent;
			secondVertex->tangent = tangent;
			thirdVertex->tangent = tangent;

			firstVertex->bitangent = bitangent;
			secondVertex->bitangent = bitangent;
			thirdVertex->bitangent = bitangent;
		} // tangents

		MeshModel* meshModel = new MeshModel(vertices->data(), vertices->size(), data->objects[i]->object_name);

		if (data->objects[i]->material != "" && data->hash_materials.find(data->objects[i]->material) != data->hash_materials.end()) {
			meshModel->material.name = data->objects[i]->material;
			meshModel->material.diffuse_texture = data->hash_materials[data->objects[i]->material]->diffuse_texture;
			meshModel->material.normal_texture = data->hash_materials[data->objects[i]->material]->normal_texture;
			meshModel->material.active = true;
		}

		mesh->objects.push_back(meshModel);
	}

	delete data;
	return mesh;
}
