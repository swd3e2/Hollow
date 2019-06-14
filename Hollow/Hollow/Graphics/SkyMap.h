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
#include "Hollow/Common/FileSystem.h"

class SkyMap
{
public:
	int NumSphereVertices;
	int NumSphereFaces;
	Hollow::FileSystem fs;
	Matrix4 transform;

	Mesh* mesh;

	SkyMap(int LatLines, int LongLines)
	{
		TEXTURE_DESC** desc = new TEXTURE_DESC * [6];
		std::vector<std::string> filenames;
		filenames.push_back("Sandbox/Resources/Textures/cubemap/cloudtop_ft.tga");
		filenames.push_back("Sandbox/Resources/Textures/cubemap/cloudtop_bk.tga");
		filenames.push_back("Sandbox/Resources/Textures/cubemap/cloudtop_up.tga");
		filenames.push_back("Sandbox/Resources/Textures/cubemap/cloudtop_dn.tga");
		filenames.push_back("Sandbox/Resources/Textures/cubemap/cloudtop_rt.tga");
		filenames.push_back("Sandbox/Resources/Textures/cubemap/cloudtop_lf.tga");
		std::vector<std::string> vec = fs.read_directory("Sandbox/Resources/Textures/cubemap/");
		for (int i = 0; i < filenames.size(); i++)
		{
			desc[i] = FreeImgImporter::instance()->import(filenames[i].c_str());
		}
		desc[0]->filename = "SkyMap";

		mesh = getCube();
		mesh->models[0]->material = new Material();
		mesh->models[0]->material->shader = ShaderManager::instance()->getShader("SkyMap");
		mesh->models[0]->material->diffuseTexture = TextureManager::instance()->Create3dTexture(desc);
	}
};