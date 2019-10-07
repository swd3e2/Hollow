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
		Hollow::s_ptr<Hollow::TextureData> desc = Hollow::FreeImgImporter::instance()->import("Sandbox/Resources/Textures/Irradiance.tga");
		Hollow::s_ptr<Hollow::Texture> cubemap = Hollow::TextureManager::instance()->create3dTexture(desc);

		mesh = getCube();
		mesh->models[0]->material = new Hollow::Material();
		mesh->models[0]->material->diffuseTexture = cubemap;
	}
};