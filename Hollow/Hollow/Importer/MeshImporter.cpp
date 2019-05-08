#include "MeshImporter.h"

Mesh* MeshImporter::import(const char* filename, bool async)
{
	Mesh* mesh = new Mesh();
	mesh->filename = filename;

	std::function<void()> task = [=]() {
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(mesh->filename.c_str(),
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);

		if (!scene) return;

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			SubMesh* sMesh = new SubMesh();
			std::vector<Vertex> vertexData;

			for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++)
			{
				Vertex vertex;
				vertex.pos.x = scene->mMeshes[i]->mVertices[j].x;
				vertex.pos.y = scene->mMeshes[i]->mVertices[j].y;
				vertex.pos.z = scene->mMeshes[i]->mVertices[j].z;

				if (scene->mMeshes[i]->HasNormals())
				{
					vertex.normal.x = scene->mMeshes[i]->mNormals[j].x;
					vertex.normal.y = scene->mMeshes[i]->mNormals[j].y;
					vertex.normal.z = scene->mMeshes[i]->mNormals[j].z;
				}

				if (scene->mMeshes[i]->HasTextureCoords(0))
				{
					vertex.texCoord.x = 1.0f - scene->mMeshes[i]->mTextureCoords[0][j].x;
					vertex.texCoord.y = 1.0f - scene->mMeshes[i]->mTextureCoords[0][j].y;
				}
				vertexData.push_back(vertex);
			}

			sMesh->vBuffer = HardwareBufferManager::instance()->createVertexBuffer(vertexData.data(), vertexData.size());

			int size = scene->mMeshes[i]->mNumFaces * scene->mMeshes[i]->mFaces[0].mNumIndices;
			unsigned int* indices = new unsigned int[size];

			for (int j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
			{
				for (int k = 0; k < scene->mMeshes[i]->mFaces[j].mNumIndices; k++)
				{
					indices[j * scene->mMeshes[i]->mFaces[0].mNumIndices + k] = scene->mMeshes[i]->mFaces[j].mIndices[k];
				}
			}

			sMesh->iBuffer = HardwareBufferManager::instance()->createIndexBuffer(indices, size);

			aiString str;

			sMesh->material = new Material();
			if (aiReturn_SUCCESS == scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &str))
			{
				sMesh->material->diffuse_texture_name = str.C_Str();
				sMesh->material->diffuse_texture = TextureManager::instance()->CreateTextureFromFile(sMesh->material->diffuse_texture_name);
			}
			if (aiReturn_SUCCESS == scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]->GetTexture(aiTextureType_HEIGHT, 0, &str))
			{
				sMesh->material->normal_texture_name = str.C_Str();
				sMesh->material->diffuse_texture = TextureManager::instance()->CreateTextureFromFile(sMesh->material->diffuse_texture_name);
			}
			if (aiReturn_SUCCESS == scene->mMaterials[scene->mMeshes[i]->mMaterialIndex]->GetTexture(aiTextureType_SPECULAR, 0, &str))
			{
				sMesh->material->specular_texture_name = str.C_Str();
				sMesh->material->diffuse_texture = TextureManager::instance()->CreateTextureFromFile(sMesh->material->diffuse_texture_name);
			}

			mesh->subMeshes.push_back(sMesh);
		}
		mesh->mNumSubmeshes = mesh->subMeshes.size();
		mesh->setReady(true);
	};
	if (async) {
		TaskManager::instance()->add(task);
	}
	else {
		task();
	}

	return mesh;
}
