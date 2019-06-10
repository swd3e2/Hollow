#include "MeshImporter.h"

#define INVALID_BONE_ID 99

MeshImportData* MeshImporter::import(const char* filename, bool async)
{
	MeshImportData* data = new MeshImportData();

	data->filename = filename;

	std::function<void()> task = [=]() {
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(data->filename.c_str(),
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_MakeLeftHanded | 
			aiProcess_GenNormals
		);

		if (!scene) return;

		// Bone data
		if (scene->HasMeshes() && scene->mMeshes[0]->HasBones() && false) {
			data->animationData = new AnimationData();
			std::unordered_map<std::string, Bone*> nodeMap;
			SetChilds(scene->mRootNode, nodeMap);
			
			int boneCounter = 0;
			for (int i = 0; i < scene->mNumMeshes; i++) {
				/* Vertex bone information */
				if (scene->mMeshes[i]->HasBones()) {
					for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++) {
						aiBone* aibone = scene->mMeshes[i]->mBones[j];
						Bone* bone = nodeMap[aibone->mName.C_Str()];
						bone->id = boneCounter++;
						bone->localTransform = Matrix4((const float*)&aibone->mOffsetMatrix, 16);
					}
				}
			}

			for (auto& it : nodeMap) {
				if (it.second->id != INVALID_BONE_ID) {
					data->animationData->bones[it.second->name] = it.second;
				}
			}
			
			data->animationData->rootBone = nodeMap[scene->mRootNode->mName.C_Str()];
			data->animationData->rootBone->childs.clear();
			data->animationData->globalInverse = Matrix4((const float*)&scene->mRootNode->mTransformation.Inverse(), 16);

			for (auto& it : data->animationData->bones) {
				if (data->animationData->bones.find(it.second->parent->name) == data->animationData->bones.end()) {
					it.second->parent = data->animationData->rootBone;
					data->animationData->rootBone->childs.push_back(it.second);
				}
			}

			// Animation
			if (scene->HasAnimations() && false) {
				for (int i = 0; i < scene->mNumAnimations; i++) {
					Animation* anim = new Animation();
					aiAnimation* animation = scene->mAnimations[i];
					anim->duration = animation->mDuration;

					for (int j = 0; j < animation->mNumChannels; j++) {
						aiNodeAnim* animNode = animation->mChannels[j];
						Bone* bone = data->animationData->bones[animNode->mNodeName.C_Str()];

						if (bone != nullptr) {
							std::vector<KeyFrame*>& frames = anim->keyFrames[animNode->mNodeName.C_Str()];

							for (int k = 0; k < animNode->mNumPositionKeys; k++) {
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
					data->animationData->animations.push_back(anim);
				}
			}
		}

		// Mesh data
		if (scene->mNumMeshes > 0)
		{
			aiNode* temp;

			data->meshData = new MeshData();
			data->meshData->numModels = scene->mNumMeshes;
			data->meshData->vertices = new std::vector<Vertex>[scene->mNumMeshes];
			data->meshData->indices = new std::vector<unsigned int>[scene->mNumMeshes];

			std::vector<LoadedMaterial*>& materials = data->meshData->materials;

			for (int i = 0; i < scene->mNumMeshes; i++) {
				aiMesh* aMesh = scene->mMeshes[i];
				data->meshData->modelNames.push_back(aMesh->mName.C_Str());
				std::vector<Vertex>& vertexData = data->meshData->vertices[i];
				std::vector<unsigned int>& indexData = data->meshData->indices[i];
				temp = FindNode(scene->mRootNode, aMesh->mName.C_Str());

				for (int j = 0; j < aMesh->mNumVertices; j++) {
					Vertex vertex;
					aiVector3D& position = aMesh->mVertices[j];
					
					if (temp != nullptr) {
						position = temp->mTransformation * position;
					}

					vertex.pos.x = position.x;
					vertex.pos.y = position.y;
					vertex.pos.z = position.z;

					if (scene->mMeshes[i]->HasNormals()) {
						aiVector3D& normal = aMesh->mNormals[j];

						vertex.normal.x = normal.x;
						vertex.normal.y = normal.y;
						vertex.normal.z = normal.z;
					}

					if (scene->mMeshes[i]->HasTextureCoords(0)) {
//#ifdef D3D11
						vertex.texCoord.x = aMesh->mTextureCoords[0][j].x;
						vertex.texCoord.y = 1.0f - aMesh->mTextureCoords[0][j].y;
//#else
//						vertex.texCoord.x = 1.0f - aMesh->mTextureCoords[0][j].x;
//						vertex.texCoord.y = 1.0f - aMesh->mTextureCoords[0][j].y;
//#endif

					}

					if (scene->mMeshes[i]->HasTangentsAndBitangents()) {
						vertex.tangent.x = aMesh->mTangents[j].x;
						vertex.tangent.y = aMesh->mTangents[j].y;
						vertex.tangent.z = aMesh->mTangents[j].z;
						vertex.bitangent.x = aMesh->mBitangents[j].x;
						vertex.bitangent.y = aMesh->mBitangents[j].y;
						vertex.bitangent.z = aMesh->mBitangents[j].z;
					}

					vertexData.push_back(vertex);
				}

				/* Indices */
				int size = scene->mMeshes[i]->mNumFaces * aMesh->mFaces[0].mNumIndices;
				unsigned int* indices = new unsigned int[size];

				for (int j = 0; j < aMesh->mNumFaces; j++) {
					for (int k = 0; k < aMesh->mFaces[j].mNumIndices; k++) {
						indexData.push_back(aMesh->mFaces[j].mIndices[k]);
					}
				}

				aiString str;

				// Material data
				aiMaterial* aMaterial = scene->mMaterials[aMesh->mMaterialIndex];

				LoadedMaterial* material = new LoadedMaterial();
				material->name = aMaterial->GetName().C_Str();

				if (aiReturn_SUCCESS == aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &str)) {
					material->diffuseTextureName = str.C_Str();
					material->hasDiffueTexture = true;
				}
				if (aiReturn_SUCCESS == aMaterial->GetTexture(aiTextureType_HEIGHT, 0, &str)) {
					material->normalTextureName = str.C_Str();
					material->hasNormalTexture = true;
				}
				if (aiReturn_SUCCESS == aMaterial->GetTexture(aiTextureType_SPECULAR, 0, &str)) {
					material->specularTextureName = str.C_Str();
					material->hasSpecularTexture = true;
				}

				materials.push_back(material);

				/* Vertex bone information */
				if (scene->mMeshes[i]->HasBones() && false) {
					for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++) {
						aiBone* aibone = scene->mMeshes[i]->mBones[j];
						Bone* bone = data->animationData->bones[aibone->mName.C_Str()];

						for (int k = 0; k < aibone->mNumWeights; k++) {
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
			}
		}

		importer.FreeScene();
	};

	if (async) {
		TaskManager::instance()->add(task);
	} else {
		task();
	}

	return data;
}

aiNode* MeshImporter::FindRootNode(aiNode* node, const std::string& name)
{
	aiNode* foundNode = nullptr;
	std::string temp = std::string(node->mName.C_Str());
	if (name.compare(temp) == 0) {
		return node;
	}

	for (int i = 0; i < node->mNumChildren; i++) {
		temp = std::string(node->mChildren[i]->mName.C_Str());
		if (name.compare(temp) == 0) {
			return node->mChildren[i];
		}
	}

	for (int i = 0; i < node->mNumChildren; i++) {
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

	for (int i = 0; i < node->mNumChildren; i++) {
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

	for (int i = 0; i < node->mNumChildren; i++) {
		temp = std::string(node->mChildren[i]->mName.C_Str());
		if (name.compare(temp) == 0) {
			return node->mChildren[i];
		}
	}
	for (int i = 0; i < node->mNumChildren; i++) {
		return FindNode(node->mChildren[i], name);
	}
	return nullptr;
}
