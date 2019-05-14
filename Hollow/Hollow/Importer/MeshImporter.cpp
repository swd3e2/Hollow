#include "MeshImporter.h"

#define INVALID_BONE_ID 99

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
			aiProcess_SortByPType |
			aiProcess_MakeLeftHanded | aiProcess_GenNormals
		);

		if (!scene) return;

		if (scene->HasMeshes() && scene->mMeshes[0]->HasBones()) {
			SetChilds(scene->mRootNode, mesh->bones);
		
			int boneCounter = 0;
			for (int i = 0; i < scene->mNumMeshes; i++)
			{
				/* Vertex bone information */
				if (scene->mMeshes[i]->HasBones())
				{
					for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++)
					{
						aiBone* aibone = scene->mMeshes[i]->mBones[j];
						Bone* bone = mesh->bones[aibone->mName.C_Str()];
						bone->id = boneCounter++;
						bone->localTransform = Matrix4((const float*)&aibone->mOffsetMatrix, 16);
					}
				}
			}

			for (auto& it : mesh->bones)
			{
				if (it.second->id < 99) 
				{
					mesh->usableBones[it.second->name] = it.second;
				}
			}

			Bone* bone = nullptr;
			for (auto& it : mesh->usableBones)
			{
				if (it.second->parent == nullptr) 
				{
					bone = it.second;
				}
			}
			if (bone == nullptr) 
			{
				for (auto& it : mesh->usableBones) 
				{
					if (mesh->usableBones.find(it.second->parent->name) == mesh->usableBones.end()) 
					{
						bone = it.second->parent;
						break;
					}
				}
				aiNode* node = FindRootNode(scene->mRootNode, bone->name);
				mesh->rootBone = mesh->bones[node->mName.C_Str()];
				mesh->m_GlobalInverseTransform = Matrix4(mesh->rootBone->localTransform);
			} else {
				aiNode* node = FindRootNode(scene->mRootNode, bone->name);
				mesh->rootBone = mesh->bones[node->mParent->mName.C_Str()];
				mesh->m_GlobalInverseTransform = Matrix4((const float*)&node->mParent->mTransformation.Inverse(), 16);
			}
		}
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			SubMesh* sMesh = new SubMesh();
			/* Vertices */
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

				if (scene->mMeshes[i]->HasTangentsAndBitangents())
				{
					vertex.tangent.x = scene->mMeshes[i]->mTangents[j].x;
					vertex.tangent.y = scene->mMeshes[i]->mTangents[j].y;
					vertex.tangent.z = scene->mMeshes[i]->mTangents[j].z;
					vertex.bitangent.x = scene->mMeshes[i]->mBitangents[j].x;
					vertex.bitangent.y = scene->mMeshes[i]->mBitangents[j].y;
					vertex.bitangent.z = scene->mMeshes[i]->mBitangents[j].z;
				}

				vertexData.push_back(vertex);
			}

			/* Indices */
			int size = scene->mMeshes[i]->mNumFaces * scene->mMeshes[i]->mFaces[0].mNumIndices;
			unsigned int* indices = new unsigned int[size];

			for (int j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
			{
				for (int k = 0; k < scene->mMeshes[i]->mFaces[j].mNumIndices; k++)
				{
					indices[j * scene->mMeshes[i]->mFaces[0].mNumIndices + k] = scene->mMeshes[i]->mFaces[j].mIndices[k];
				}
			}

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

			/* Vertex bone information */
			if (scene->mMeshes[i]->HasBones())
			{
				for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++)
				{
					aiBone* aibone = scene->mMeshes[i]->mBones[j];
					Bone* bone = mesh->usableBones[aibone->mName.C_Str()];

					for (int k = 0; k < aibone->mNumWeights; k++)
					{
						int vertexId = aibone->mWeights[k].mVertexId;
						float weight = aibone->mWeights[k].mWeight;

						for (int l = 0; l < 4; l++) {
							if (vertexData[vertexId].boneData.Weights[l] == 0.0) {
								vertexData[vertexId].boneData.IDs[l] = bone->id;
								vertexData[vertexId].boneData.Weights[l] = weight;
								break;
							}
						}
					}
				}
			}

			sMesh->iBuffer = HardwareBufferManager::instance()->createIndexBuffer(indices, size);
			sMesh->vBuffer = HardwareBufferManager::instance()->createVertexBuffer(vertexData.data(), vertexData.size());

			mesh->subMeshes.push_back(sMesh);
		}

		if (scene->HasAnimations()) {
			for (int i = 0; i < scene->mNumAnimations; i++)
			{
				Animation* anim = new Animation();
				aiAnimation* animation = scene->mAnimations[i];
				anim->duration = animation->mDuration;

				for (int j = 0; j < animation->mNumChannels; j++)
				{
					aiNodeAnim* animNode = animation->mChannels[j];
					Bone* bone = mesh->bones[animNode->mNodeName.C_Str()];
				
					if (bone != nullptr) {
						std::vector<KeyFrame*>& frames = anim->keyFrames[animNode->mNodeName.C_Str()];

						for (int k = 0; k < animNode->mNumPositionKeys; k++)
						{
							KeyFrame* keyFrame = new KeyFrame();
							keyFrame->transform.translate.x = animNode->mPositionKeys[k].mValue.x;
							keyFrame->transform.translate.y = animNode->mPositionKeys[k].mValue.y;
							keyFrame->transform.translate.z = animNode->mPositionKeys[k].mValue.z;

							keyFrame->transform.rotation.x = animNode->mRotationKeys[k].mValue.x;
							keyFrame->transform.rotation.y = animNode->mRotationKeys[k].mValue.y;
							keyFrame->transform.rotation.z = animNode->mRotationKeys[k].mValue.z;
							keyFrame->transform.rotation.w = animNode->mRotationKeys[k].mValue.w;

							keyFrame->transform.scale.x = animNode->mScalingKeys[k].mValue.x;
							keyFrame->transform.scale.y = animNode->mScalingKeys[k].mValue.y;
							keyFrame->transform.scale.z = animNode->mScalingKeys[k].mValue.z;
							
							keyFrame->time = animNode->mPositionKeys[k].mTime;
							keyFrame->boneId = bone->id;
							keyFrame->index = k;

							frames.push_back(keyFrame);
						}
					}
				}
				mesh->animations.push_back(anim);
			}
		}

		mesh->mNumSubmeshes = mesh->subMeshes.size();
		mesh->setReady(true);
	};

	if (async) {
		TaskManager::instance()->add(task);
	} else {
		task();
	}

	return mesh;
}

aiNode* MeshImporter::FindRootNode(aiNode* node, const std::string& name)
{
	aiNode* foundNode = nullptr;
	std::string temp = std::string(node->mName.C_Str());
	if (name.compare(temp) == 0)
	{
		return node;
	}
	for (int i = 0; i < node->mNumChildren; i++)
	{
		temp = std::string(node->mChildren[i]->mName.C_Str());
		if (name.compare(temp) == 0)
		{
			return node->mChildren[i];
		}
	}
	for (int i = 0; i < node->mNumChildren; i++)
	{
		foundNode = FindRootNode(node->mChildren[i], name);
	}
	return foundNode;
}

void MeshImporter::SetChilds(aiNode* node, std::unordered_map<std::string, Bone*>& bones)
{
	Bone* rootBone = bones[node->mName.C_Str()];

	if (rootBone == nullptr) {
		rootBone = new Bone();
		rootBone->name = node->mName.C_Str();
		rootBone->id = INVALID_BONE_ID;
		rootBone->localTransform = Matrix4::Identity();
		bones[node->mName.C_Str()] = rootBone;
	}

	for (int i = 0; i < node->mNumChildren; i++) 
	{
		Bone* bone = bones[node->mChildren[i]->mName.C_Str()];

		if (bone == nullptr) {
			bone = new Bone();
			bone->name = node->mChildren[i]->mName.C_Str();
			bone->id = INVALID_BONE_ID;
			bone->localTransform = Matrix4::Identity();

			bones[node->mChildren[i]->mName.C_Str()] = bone;
		}

		bone->parent = rootBone;
		rootBone->childs.push_back(bone);
		SetChilds(node->mChildren[i], bones);
	}
}

aiNode* MeshImporter::FindNode(aiNode* node, const std::string& name)
{
	std::string temp;

	for (int i = 0; i < node->mNumChildren; i++)
	{
		temp = std::string(node->mChildren[i]->mName.C_Str());
		if (name.compare(temp) == 0)
		{
			return node->mChildren[i];
		}
	}
	for (int i = 0; i < node->mNumChildren; i++)
	{
		return FindNode(node->mChildren[i], name);
	}
	return nullptr;
}
