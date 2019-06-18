#pragma once

#include <Hollow/Math/Matrix4.h>
#include <Hollow/Resources/Mesh/Mesh.h>
#include <Hollow/Graphics/Vertex.h>
#include <Hollow/Graphics/HardwareBufferManager.h>
#include <Hollow/Graphics/TextureManager.h>
#include <Hollow/Importer/FreeImgImporter.h>
#include <Hollow/Graphics/ShaderManager.h>
#include <Hollow/Test.h>

class SkyMap
{
public:
	int NumSphereVertices;
	int NumSphereFaces;
	Hollow::Matrix4 transform;

	Hollow::Mesh* mesh;

	SkyMap()
	{
		/*Hollow::TEXTURE_DESC** desc = new Hollow::TEXTURE_DESC * [6];

		std::vector<std::string> filenames;
		filenames.push_back("Sandbox/Resources/Textures/cubemap/cloudtop_ft.tga");
		filenames.push_back("Sandbox/Resources/Textures/cubemap/cloudtop_bk.tga");
		filenames.push_back("Sandbox/Resources/Textures/cubemap/cloudtop_up.tga");
		filenames.push_back("Sandbox/Resources/Textures/cubemap/cloudtop_dn.tga");
		filenames.push_back("Sandbox/Resources/Textures/cubemap/cloudtop_rt.tga");
		filenames.push_back("Sandbox/Resources/Textures/cubemap/cloudtop_lf.tga");

		for (int i = 0; i < filenames.size(); i++) {
			desc[i] = Hollow::FreeImgImporter::instance()->import(filenames[i].c_str());
		}
		desc[0]->filename = "SkyMap";
*/

		Hollow::TEXTURE_DESC* desc = Hollow::FreeImgImporter::instance()->import("Sandbox/Resources/Textures/cube.jpg");
		Hollow::Texture* cubemap = Hollow::TextureManager::instance()->Create3dTexture(desc);

		mesh = getCube();
		mesh->models[0]->material = new Hollow::Material();
		mesh->models[0]->material->shader = Hollow::ShaderManager::instance()->getShader("SkyMap");
		mesh->models[0]->material->diffuseTexture = cubemap;
	}
};