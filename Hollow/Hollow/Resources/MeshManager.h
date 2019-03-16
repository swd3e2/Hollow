#pragma once
#include "Hollow/Platform.h"
#include "Hollow/Core/CModule.h"
#include "Utils/ObjParser.h"
#include "Mesh.h"
#include <d3d11.h>

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
};