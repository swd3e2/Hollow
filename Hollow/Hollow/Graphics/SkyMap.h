#pragma once

#include "Hollow/Math/Matrix4.h"
#include "Hollow/Resources/Mesh/Mesh.h"
#include "Vertex.h"
#include "HardwareBufferManager.h"
#include "TextureManager.h"
#include "Hollow/Importer/FreeImgImporter.h"
#include "ShaderManager.h"
#include "Hollow/Importer/MeshImporter.h"
#include "Hollow/Test.h"

class SkyMap
{
public:
	int NumSphereVertices;
	int NumSphereFaces;

	Matrix4 transform;

	Mesh* mesh;

	SkyMap(int LatLines, int LongLines)
	{
		TEXTURE_DESC** desc = new TEXTURE_DESC * [6];
		std::vector<std::string> filenames;
		filenames.push_back("Sandbox/Resources/Textures/cubemap/arctic_ft.tga");
		filenames.push_back("Sandbox/Resources/Textures/cubemap/arctic_bk.tga");
		filenames.push_back("Sandbox/Resources/Textures/cubemap/arctic_up.tga");
		filenames.push_back("Sandbox/Resources/Textures/cubemap/arctic_dn.tga");
		filenames.push_back("Sandbox/Resources/Textures/cubemap/arctic_rt.tga");
		filenames.push_back("Sandbox/Resources/Textures/cubemap/arctic_lf.tga");

		for (int i = 0; i < filenames.size(); i++)
		{
			desc[i] = FreeImgImporter::import(filenames[i].c_str());
		}

		mesh = getCube();
		mesh->subMeshes[0]->material = new Material();
		mesh->subMeshes[0]->material->shader = ShaderManager::instance()->getShader("SkyMap");
		mesh->subMeshes[0]->material->diffuse_texture = TextureManager::instance()->Create3dTexture(desc);
	}
};