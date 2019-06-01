#pragma once 

#include "Hollow/Resources/Mesh/Mesh.h"
#include "Hollow/Resources/Mesh/Animation.h"
#include "Hollow/Resources/Mesh/Bone.h"
#include "Hollow/Graphics/Vertex.h"
#include <string>
#include <unordered_map>
#include "Hollow/Math/Matrix4.h"

struct LoadedMaterial
{
	std::string name;
	std::string diffuseTextureName;
	std::string normalTextureName;
	std::string specularTextureName;

	bool hasDiffueTexture = false;
	bool hasNormalTexture = false;
	bool hasSpecularTexture = false;
};

struct AnimationData
{
	Bone* rootBone;
	std::unordered_map<std::string, Bone*> bones;
	std::vector<Animation*> animations;
	Matrix4 globalInverse;
};

struct MeshData
{
	std::vector<Vertex>* vertices;
	std::vector<unsigned int>* indices;
	std::vector<LoadedMaterial*> materials;
	std::vector<std::string> modelNames;
	int numModels;
};

class MeshImportData
{
public:
	MeshData* meshData;
	AnimationData* animationData;
	std::string filename;
public:
	MeshImportData() :
		meshData(nullptr), animationData(nullptr) 
	{}
	inline bool hasAnimation() { return animationData != nullptr; }
	inline bool hasMesh() { return meshData != nullptr; }
};