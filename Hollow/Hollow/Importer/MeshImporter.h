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
#include "MeshImportData.h"

class HOLLOW_API MeshImporter
{
public:
	static void import(const char* filename, MeshImportData* data, bool async = true);
private:
	static aiNode* FindRootNode(aiNode* node, const std::string& name);
	static aiNode* FindNode(aiNode* node, const std::string& name);
	static void SetChilds(aiNode* node, std::unordered_map<std::string, Bone*>& bones);
};

#endif