#pragma once

#ifndef HW_MESH_H
#define HW_MESH_H

#include "Model.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Importer/MeshImportData.h"
#include "Hollow/Math/Matrix4.h"
#include "Hollow/Resources/Material.h"
#include "Hollow/Graphics/HardwareBufferManager.h"
#include "Hollow/Graphics/TextureManager.h"

class  Mesh
{
public:
	int numModels;
	std::vector<Model*> models;
	std::string filename;
public:
	Mesh() : numModels(0) {}

	Mesh(MeshData* data)
	{
		numModels = data->numModels;

		for (int i = 0; i < data->numModels; i++)
		{
			Model* model = new Model();

			model->vBuffer = HardwareBufferManager::instance()->createVertexBuffer(data->vertices[i].data(), data->vertices[i].size());
			model->iBuffer = HardwareBufferManager::instance()->createIndexBuffer(data->indices[i].data(), data->indices[i].size());

			// Material data
			model->material = new Material();
			if (data->materials[i]->hasDiffueTexture) {
				model->material->diffuse_texture = TextureManager::instance()->CreateTextureFromFile(data->materials[i]->diffuseTextureName);
			}
			if (data->materials[i]->hasNormalTexture) {
				model->material->normal_texture = TextureManager::instance()->CreateTextureFromFile(data->materials[i]->normalTextureName);
			}
			if (data->materials[i]->hasSpecularTexture) {
				model->material->specular_texture = TextureManager::instance()->CreateTextureFromFile(data->materials[i]->specularTextureName);
			}
			models.push_back(model);
		}
	}
};

#endif