#pragma once

#ifndef HW_MESH_IMPORTER_H
#define HW_MESH_IMPORTER_H

#include "Hollow/Resources/Mesh/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>           
#include <assimp/postprocess.h>    
#include "Hollow/Graphics/Vertex.h"
#include "Hollow/Containers/vector.h"
#include <vector>
#include "Hollow/Graphics/HardwareBufferManager.h"
#include "Hollow/Graphics/TextureManager.h"
#include "Hollow/Core/TaskManager.h"
#include "Hollow/Platform.h"

class HOLLOW_API MeshImporter
{
public:
	static Mesh* import(const char* filename, bool async = true);
private:
	static aiNode* FindRootNode(aiNode* node, std::unordered_map<std::string, Bone*>& bones);
	static void SetChilds(aiNode* node, std::unordered_map<std::string, Bone*>& bones);
	Bone* FindBone(const std::string& boneName, Mesh* mesh);
};

#endif