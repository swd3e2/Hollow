#pragma once

#include "vendor/assimp/assimp/Importer.hpp"
#include "vendor/assimp/assimp/scene.h"     
#include "vendor/assimp/assimp/postprocess.h"
#include <vector>
#include "Hollow/Math/Vector4.h"
#include "Hollow/Math/Vector3.h"
#include "Hollow/Math/Vector2.h"
#include <unordered_map>
#include "Mesh.h"

namespace Hollow {
	class AssimpImporter
	{
	public:
		AssimpImporter() {  }
		~AssimpImporter() {  }

		Import::Model* import(const char* filename, bool async = true)
		{
			Import::Model* data = new Import::Model();

			Assimp::Importer importer;

			const aiScene* scene = importer.ReadFile(filename,
				aiProcessPreset_TargetRealtime_Quality
			);

			if (!scene) return nullptr;

			// Mesh data
			if (scene->mNumMeshes > 0) {
				aiNode* temp;

				for (int i = 0; i < scene->mNumMeshes; i++) {
					aiMesh* aMesh = scene->mMeshes[i];
					Import::Mesh* mesh = new Import::Mesh();
					mesh->name = aMesh->mName.C_Str();
					mesh->material = aMesh->mMaterialIndex;

					for (int j = 0; j < aMesh->mNumVertices; j++) {
						Vertex vertex;
						aiVector3D& position = aMesh->mVertices[j];

						vertex.pos.x = position.x;
						vertex.pos.y = position.y;
						vertex.pos.z = position.z;

						// Find lowest and highest vertices for AABB culling
						if (vertex.pos.x < data->A.x && vertex.pos.y < data->A.y && vertex.pos.z < data->A.z) {
							data->A = vertex.pos;
						}
						if (vertex.pos.x > data->B.x && vertex.pos.y > data->B.y && vertex.pos.z > data->B.z) {
							data->B = vertex.pos;
						}

						if (scene->mMeshes[i]->HasNormals()) {
							aiVector3D& normal = aMesh->mNormals[j];

							vertex.normal.x = normal.x;
							vertex.normal.y = normal.y;
							vertex.normal.z = normal.z;
						}

						if (scene->mMeshes[i]->HasTextureCoords(0)) {
							vertex.texCoord.x = aMesh->mTextureCoords[0][j].x;
							vertex.texCoord.y = 1.0f - aMesh->mTextureCoords[0][j].y;
						}

						if (scene->mMeshes[i]->HasTangentsAndBitangents()) {
							vertex.tangent.x = aMesh->mTangents[j].x;
							vertex.tangent.y = aMesh->mTangents[j].y;
							vertex.tangent.z = aMesh->mTangents[j].z;
							vertex.bitangent.x = aMesh->mBitangents[j].x;
							vertex.bitangent.y = aMesh->mBitangents[j].y;
							vertex.bitangent.z = aMesh->mBitangents[j].z;
						}

						mesh->vertices.push_back(vertex);
					}

					/* Indices */
					int size = scene->mMeshes[i]->mNumFaces * aMesh->mFaces[0].mNumIndices;
					unsigned int* indices = new unsigned int[size];

					for (int j = 0; j < aMesh->mNumFaces; j++) {
						for (int k = 0; k < aMesh->mFaces[j].mNumIndices; k++) {
							mesh->indices.push_back(aMesh->mFaces[j].mIndices[k]);
						}
					}

					if (data->materials.find(mesh->material) == data->materials.end()) {
						aiString str;

						// Material data
						aiMaterial* aMaterial = scene->mMaterials[aMesh->mMaterialIndex];
					
					
						Import::Material material;
						material.name = aMaterial->GetName().C_Str();

						aiColor3D color(0.f, 0.f, 0.f);
						aMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
						material.baseColorFactor = Vector4(color.r, color.g, color.b, 1.0f);


						if (aiReturn_SUCCESS == aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &str)) {
							material.diffuseTexture = str.C_Str();
						}
						if (aiReturn_SUCCESS == aMaterial->GetTexture(aiTextureType_HEIGHT, 0, &str)) {
							material.normalTexture = str.C_Str();
						}
						if (aiReturn_SUCCESS == aMaterial->GetTexture(aiTextureType_SPECULAR, 0, &str)) {
							material.specularTexture = str.C_Str();
						}

						data->materials[mesh->material] = material;
					}
					data->meshes.push_back(mesh);
				}
			}

			importer.FreeScene();

			return data;
		}
	};

}