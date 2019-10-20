#pragma once

#include "Hollow/Common/Log.h"
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
	private:
		unsigned int animationNodeNextId = 0;
	public:
		AssimpImporter() {  }
		~AssimpImporter() {  }

		s_ptr<Import::Model> import(const char* filename, bool async = true)
		{
			animationNodeNextId = 0;

			s_ptr<Import::Model> model(new Import::Model());

			Assimp::Importer importer;

			const aiScene* scene = importer.ReadFile(filename,
				aiProcessPreset_TargetRealtime_Quality
			);

			if (!scene) 
				return model;

			std::unordered_map<std::string, Import::AnimationNode*> animationNodes;
			
			if (scene->mNumAnimations > 0) {
				parseAnimationData(animationNodes, model, scene);
			}

			parseCommonData(animationNodes, model, scene);

			importer.FreeScene();

			return model;
		}
		private:
		// Mesh data
		void parseCommonData(std::unordered_map<std::string, Import::AnimationNode*>& animationNodes, const s_ptr<Import::Model>& data, const aiScene* scene)
		{
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
						if (vertex.pos.x > data->B.x&& vertex.pos.y > data->B.y&& vertex.pos.z > data->B.z) {
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

					if (scene->mMeshes[i]->HasBones()){
						for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++) {
							aiBone* aibone = scene->mMeshes[i]->mBones[j];
							
							if (animationNodes.find(aibone->mName.C_Str()) == animationNodes.end()){
								HW_ERROR("Animation node presented in mesh not found in nodes list, {}", aibone->mName.C_Str());
								continue;
							}

							Import::AnimationNode* node = animationNodes[aibone->mName.C_Str()];

							for (int k = 0; k < aibone->mNumWeights; k++) {
								int vertexId = aibone->mWeights[k].mVertexId;
								float weight = aibone->mWeights[k].mWeight;

								for (int l = 0; l < 4; l++) {
									if (mesh->vertices[vertexId].boneData.weights[l] == 0.0) {
										mesh->vertices[vertexId].boneData.joints[l] = node->id;
										mesh->vertices[vertexId].boneData.weights[l] = weight;
										break;
									}
								}
							}
						}
					}

					data->meshes.push_back(mesh);
				}
			}
		}

		void parseAnimationData(std::unordered_map<std::string, Import::AnimationNode*>& animationNodes, s_ptr<Import::Model>& data, const aiScene* scene)
		{
			// Create assimp node map 
			std::unordered_map<std::string, aiNode*> assimpNodes;
			createAssimpNodeMap(assimpNodes, scene->mRootNode);

			// Create list of bones that used as bones
			std::unordered_map<std::string, aiNode*> assimpBoneNodes;
			std::unordered_map<std::string, aiBone*> assimpBones;

			for (int i = 0; i < scene->mNumMeshes; i++) {
				if (scene->mMeshes[i]->HasBones()) {
					for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++) {
						aiBone* aibone = scene->mMeshes[i]->mBones[j];
						if (assimpNodes.find(aibone->mName.C_Str()) == assimpNodes.end()) {
							HW_ERROR("Bone not found in nodes list, {}", aibone->mName.C_Str());
							assert(false);
						}
						assimpBoneNodes[scene->mMeshes[i]->mBones[j]->mName.C_Str()] = assimpNodes[aibone->mName.C_Str()];
						assimpBones[scene->mMeshes[i]->mBones[j]->mName.C_Str()] = scene->mMeshes[i]->mBones[j];
					}
				}
			}

			// Iterate up to root till find the node that isn't in bone list
			aiNode* rootNode = assimpBoneNodes.begin()->second;

			while (hasParentBone(assimpBones, rootNode)) {
				rootNode = rootNode->mParent;
			}


			aiBone* rootBone = assimpBones[rootNode->mName.C_Str()];
			data->rootNode = new Import::AnimationNode();
			data->rootNode->id = animationNodeNextId++;
			data->rootNode->name = rootNode->mName.C_Str();

			data->rootNode->localTransform = Matrix4(
				rootBone->mOffsetMatrix.a1, rootBone->mOffsetMatrix.a2, rootBone->mOffsetMatrix.a3, rootBone->mOffsetMatrix.a4,
				rootBone->mOffsetMatrix.b1, rootBone->mOffsetMatrix.b2, rootBone->mOffsetMatrix.b3, rootBone->mOffsetMatrix.b4,
				rootBone->mOffsetMatrix.c1, rootBone->mOffsetMatrix.c2, rootBone->mOffsetMatrix.c3, rootBone->mOffsetMatrix.c4,
				rootBone->mOffsetMatrix.d1, rootBone->mOffsetMatrix.d2, rootBone->mOffsetMatrix.d3, rootBone->mOffsetMatrix.d4
			);

			animationNodes[rootNode->mName.C_Str()] = data->rootNode;
			createAnimationNodes(assimpBoneNodes, assimpBones, animationNodes, rootNode, data->rootNode);

			for (int i = 0; i < scene->mNumAnimations; i++) {
				aiAnimation* assimpAnimation = scene->mAnimations[i];
				Import::Animation* animation = new Import::Animation();

				for (int j = 0; j < assimpAnimation->mNumChannels; j++) {
					aiNodeAnim* assimpChannel = assimpAnimation->mChannels[j];

					if (animationNodes.find(assimpChannel->mNodeName.C_Str()) == animationNodes.end()) {
						HW_ERROR("Could not find animation node in nodes list, {}", assimpChannel->mNodeName.C_Str());
						continue;
					}

					Import::AnimationNode* node = animationNodes[assimpChannel->mNodeName.C_Str()];
					assimpBoneNodes[assimpChannel->mNodeName.C_Str()] = assimpNodes[assimpChannel->mNodeName.C_Str()];
					Import::AnimationNodeData* animationData = nullptr;

					if (animation->data.find(node->id) != animation->data.end()) {
						animationData = animation->data[node->id];
					} else {
						animation->data[node->id] = animationData = new Import::AnimationNodeData;
					}

					for (int k = 0; k < assimpChannel->mNumPositionKeys; k++) {
						animationData->positions[assimpChannel->mPositionKeys[k].mTime] = Vector3(
							assimpChannel->mPositionKeys[k].mValue.x, 
							assimpChannel->mPositionKeys[k].mValue.y, 
							assimpChannel->mPositionKeys[k].mValue.z
						);
					}

					for (int k = 0; k < assimpChannel->mNumRotationKeys; k++) {
						animationData->rotations[assimpChannel->mRotationKeys[k].mTime] = Quaternion(
							assimpChannel->mRotationKeys[k].mValue.x,
							assimpChannel->mRotationKeys[k].mValue.y,
							assimpChannel->mRotationKeys[k].mValue.z, 
							assimpChannel->mRotationKeys[k].mValue.w
						);
					}

					for (int k = 0; k < assimpChannel->mNumScalingKeys; k++) {
						animationData->scale[assimpChannel->mScalingKeys[k].mTime] = Vector3(
							assimpChannel->mScalingKeys[k].mValue.x, 
							assimpChannel->mScalingKeys[k].mValue.y, 
							assimpChannel->mScalingKeys[k].mValue.z
						);
					}
				}
				data->animations.push_back(animation);
			}
		}

		void createAnimationNodes(std::unordered_map<std::string, aiNode*>& assimpBoneNodes, std::unordered_map<std::string, aiBone*>& assimpBones, std::unordered_map<std::string, Import::AnimationNode*>& animationNodes, const aiNode* node, Import::AnimationNode* parentNode)
		{
			for (int i = 0; i < node->mNumChildren; i++) {
				if (!hasChildBone(assimpBoneNodes, node->mChildren[i]))
					continue;

				Import::AnimationNode* animNode = new Import::AnimationNode();
				animNode->name = node->mChildren[i]->mName.C_Str();

				// If no vertex uses that bone we dont need it's id
				if (assimpBoneNodes.find(animNode->name) != assimpBoneNodes.end()) {
					animNode->id = animationNodeNextId++;
				} else {
					animNode->id = -1;
				}

				if (assimpBones.find(animNode->name) != assimpBones.end()) {
					aiBone* bone = assimpBones[animNode->name];
					animNode->localTransform = Matrix4(
						bone->mOffsetMatrix.a1, bone->mOffsetMatrix.a2, bone->mOffsetMatrix.a3, bone->mOffsetMatrix.a4,
						bone->mOffsetMatrix.b1, bone->mOffsetMatrix.b2, bone->mOffsetMatrix.b3, bone->mOffsetMatrix.b4,
						bone->mOffsetMatrix.c1, bone->mOffsetMatrix.c2, bone->mOffsetMatrix.c3, bone->mOffsetMatrix.c4,
						bone->mOffsetMatrix.d1, bone->mOffsetMatrix.d2, bone->mOffsetMatrix.d3, bone->mOffsetMatrix.d4
					);
				} else {
					// @todo: maybe set transform from aiNode
					animNode->localTransform = Matrix4::identity();
				}
				

				animationNodes[node->mChildren[i]->mName.C_Str()] = animNode;
				parentNode->childrens.push_back(animNode);

				createAnimationNodes(assimpBoneNodes, assimpBones, animationNodes, node->mChildren[i], animNode);
			}
		}

		void createAssimpNodeMap(std::unordered_map<std::string, aiNode*>& assimpNodes, const aiNode* node)
		{
			for (int i = 0; i < node->mNumChildren; i++) {
				assimpNodes[node->mChildren[i]->mName.C_Str()] = node->mChildren[i];
				createAssimpNodeMap(assimpNodes, node->mChildren[i]);
			}
		}

		bool hasChildBone(std::unordered_map<std::string, aiNode*>& assimpBoneNodes, aiNode* node)
		{
			if (assimpBoneNodes.find(node->mName.C_Str()) != assimpBoneNodes.end()) {
				return true;
			}

			bool result = false;

			for (int i = 0; i < node->mNumChildren; i++) {
				result = result || assimpBoneNodes.find(node->mChildren[i]->mName.C_Str()) != assimpBoneNodes.end();
				result = result || hasChildBone(assimpBoneNodes, node->mChildren[i]);
			}

			return result;
		}

		bool hasParentBone(std::unordered_map<std::string, aiBone*>& assimpBones, aiNode* node)
		{
			bool result = false;

			aiNode* temp = node;
			while (temp->mParent != nullptr) {
				result = result || assimpBones.find(temp->mParent->mName.C_Str()) != assimpBones.end();
				temp = temp->mParent;
			}

			return result;
		}
	};
}