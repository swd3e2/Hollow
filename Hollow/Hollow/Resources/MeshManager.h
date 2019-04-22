#pragma once

#ifndef HW_MESH_MANAGER_H
#define HW_MESH_MANAGER_H

#include "Hollow/Platform.h"
#include "Hollow/Core/CModule.h"
#include "Utils/ObjParser.h"
#include "Hollow/Math/Vector4.h"
#include "Mesh.h"

using namespace Hollow;
using namespace DirectX;

class HOLLOW_API MeshManager : public CModule<MeshManager>
{
private:
	ObjParser objParser;
public:
	void startUp();
	void shutdown();

	Mesh* CreateMesh(std::string filename, bool inversceTexCoords = false);
	Mesh* CreateMeshI(std::string filename, bool inversceTexCoords = false);
};

#endif