#include "MeshManager.h"

void MeshManager::startUp()
{
	setStartedUp();
}

void MeshManager::shutdown()
{
	setShutdown();
}

Mesh* MeshManager::CreateMesh(std::string filename, bool inversceTexCoords)
{
	MeshData* data = objParser.LoadObj(filename);
	Mesh* mesh = new Mesh(filename);

	if (data == nullptr) {
		return nullptr;
	}

	for (int i = 0; i < data->objects.size(); i++) {
		Containers::Vector<Vertex>* vertices = new Containers::Vector<Vertex>;

		for (int j = 0; j < data->objects[i]->indices.size(); j++) {
			Vertex vertex;
			vertex.pos.x = data->vertices[data->objects[i]->indices[j].vertice_index * 3];
			vertex.pos.y = data->vertices[1 + data->objects[i]->indices[j].vertice_index * 3];
			vertex.pos.z = data->vertices[2 + data->objects[i]->indices[j].vertice_index * 3];

			if (data->objects[i]->has_texture) {
				if (inversceTexCoords) {
					vertex.texCoord.x = 1 - data->tex_coords[data->objects[i]->indices[j].tex_coord_index * 2];
					vertex.texCoord.y = 1 - data->tex_coords[1 + data->objects[i]->indices[j].tex_coord_index * 2];
				} else {
					vertex.texCoord.x = data->tex_coords[data->objects[i]->indices[j].tex_coord_index * 2];
					vertex.texCoord.y = data->tex_coords[1 + data->objects[i]->indices[j].tex_coord_index * 2];
				}
			} else {
				vertex.texCoord.x = 0.0f;
				vertex.texCoord.y = 0.0f;
			}

			if (data->objects[i]->indices[j].normal_index > 0 && data->objects[i]->indices[j].normal_index * 3 + 2 < data->normals.size()) {
				vertex.normal.x = data->normals[data->objects[i]->indices[j].normal_index * 3];
				vertex.normal.y = data->normals[1 + data->objects[i]->indices[j].normal_index * 3];
				vertex.normal.z = data->normals[2 + data->objects[i]->indices[j].normal_index * 3];
			}

			vertices->push_back(vertex);
		}

		// tangents
		for (int j = 0; j < vertices->size(); j += 3)
		{
			Vertex* firstVertex = &(*vertices)[j];
			Vertex* secondVertex = &(*vertices)[j + 1];
			Vertex* thirdVertex = &(*vertices)[j + 2];

			//Get the vector describing one edge of our triangle (edge 0,2)
			Vector4 edge1 (
				secondVertex->pos.x - firstVertex->pos.x,
				secondVertex->pos.y - firstVertex->pos.y,
				secondVertex->pos.z - firstVertex->pos.z,
				0.0f
			);    //Create our first edge

			//Get the vector describing another edge of our triangle (edge 2,1)
			Vector4 edge2(
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
			Vector3 tangent(0.0f, 0.0f, 0.0f);
			float r = 1.0f / (deltaUV1x * deltaUV2y - deltaUV2x * deltaUV1y);

			tangent.x = (deltaUV2y * edge1.x - deltaUV1y * edge2.x) * r;
			tangent.y = (deltaUV2y * edge1.y - deltaUV1y * edge2.y) * r;
			tangent.z = (deltaUV2y * edge1.z - deltaUV1y * edge2.z) * r;

			Vector3 bitangent(0.0f, 0.0f, 0.0f);
			bitangent.x = (deltaUV1x * edge2.x - deltaUV2x * edge1.x) * r;
			bitangent.y = (deltaUV1x * edge2.y - deltaUV2x * edge1.y) * r;
			bitangent.z = (deltaUV1x * edge2.z - deltaUV2x * edge1.z) * r;

			firstVertex->tangent = tangent;
			secondVertex->tangent = tangent;
			thirdVertex->tangent = tangent;

			firstVertex->bitangent = bitangent;
			secondVertex->bitangent = bitangent;
			thirdVertex->bitangent = bitangent;
		} // tangents

		MeshModel* meshModel = new MeshModel(vertices->data(), vertices->size(), data->objects[i]->object_name);

		// fill material
		if (data->objects[i]->material != "" && data->hash_materials.find(data->objects[i]->material) != data->hash_materials.end()) {
			meshModel->material.name = data->objects[i]->material;
			meshModel->material.diffuse_texture = data->hash_materials[data->objects[i]->material]->diffuse_texture;
			meshModel->material.normal_texture = data->hash_materials[data->objects[i]->material]->normal_texture;
			meshModel->material.specular_texture = data->hash_materials[data->objects[i]->material]->specular_texture;

			meshModel->material.active = true;	

			if (meshModel->material.diffuse_texture.length() != 0)
				meshModel->material.materialData.hasDiffuseTexture = true;

			if (meshModel->material.normal_texture.length() != 0)
				meshModel->material.materialData.hasNormalMap = true;

			if (meshModel->material.specular_texture.length() != 0)
				meshModel->material.materialData.hasSpecularMap = true;

			meshModel->material.materialData.Ns = data->hash_materials[data->objects[i]->material]->Ns;
			meshModel->material.materialData.Ka[0] = data->hash_materials[data->objects[i]->material]->Ka[0];
			meshModel->material.materialData.Ka[1] = data->hash_materials[data->objects[i]->material]->Ka[1];
			meshModel->material.materialData.Ka[2] = data->hash_materials[data->objects[i]->material]->Ka[2];

			meshModel->material.materialData.Kd[0] = data->hash_materials[data->objects[i]->material]->Kd[0];
			meshModel->material.materialData.Kd[1] = data->hash_materials[data->objects[i]->material]->Kd[1];
			meshModel->material.materialData.Kd[2] = data->hash_materials[data->objects[i]->material]->Kd[2];

			meshModel->material.materialData.Ke[0] = data->hash_materials[data->objects[i]->material]->Ke[0];
			meshModel->material.materialData.Ke[1] = data->hash_materials[data->objects[i]->material]->Ke[1];
			meshModel->material.materialData.Ke[2] = data->hash_materials[data->objects[i]->material]->Ke[2];

			meshModel->material.materialData.Ks[0] = data->hash_materials[data->objects[i]->material]->Ks[0];
			meshModel->material.materialData.Ks[1] = data->hash_materials[data->objects[i]->material]->Ks[1];
			meshModel->material.materialData.Ks[2] = data->hash_materials[data->objects[i]->material]->Ks[2];
		}

		mesh->objects.push_back(meshModel);
	}
	delete data;
	return mesh;
}

Mesh* MeshManager::CreateMeshI(std::string filename, bool inversceTexCoords)
{
	
	return nullptr;
}