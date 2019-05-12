#pragma once

#include "Hollow/Resources/Mesh/Mesh.h"
#include "Hollow/Test.h"
#include "Hollow/Importer/FreeImgImporter.h"

class HOLLOW_API Water
{
public:
	Mesh* mesh;
	Shader* computeShader;
	Texture* tex;
public:
	Water()
	{
		mesh = new Mesh();
		SubMesh* sMesh = new SubMesh();

		Vertex cube_vertices[] = {
			Vertex( 1.0,  0.0f,  1.0,	1.0, 0.0,	1.0, -1.0, -1.0),
			Vertex(-1.0,  0.0f,  1.0,	0.0, 0.0,   1.0, -1.0, -1.0),
			Vertex(-1.0, -0.0f, -1.0,	0.0, 1.0,	1.0, -1.0, -1.0),
			Vertex( 1.0, -0.0f, -1.0,	1.0, 1.0,	1.0, -1.0, -1.0),
		};

		sMesh->vBuffer = HardwareBufferManager::instance()->createVertexBuffer(cube_vertices, 4);

		unsigned int cube_elements[] = {
			2, 1, 0,
			0, 3, 2
		};
		sMesh->iBuffer = HardwareBufferManager::instance()->createIndexBuffer(cube_elements, 6);
		mesh->subMeshes.push_back(sMesh);
		
		mesh->setReady(true);

		mesh->subMeshes[0]->material = new Material();
		mesh->subMeshes[0]->material->shader = ShaderManager::instance()->getShader("Water");
		TEXTURE_DESC* desc = FreeImgImporter::import("Sandbox/Resources/Textures/heightmaps/1.png");
		mesh->subMeshes[0]->material->diffuse_texture = TextureManager::instance()->Create2dTexture(desc);
		mesh->subMeshes[0]->material->shader->setHullShader(ShaderManager::instance()->compileShader(ShaderType::HULL, 
			"C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/hull/shader.hlsl"));
		mesh->subMeshes[0]->material->shader->setDomainShader(ShaderManager::instance()->compileShader(ShaderType::DOMAINS,
			"C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/domain/shader.hlsl"));

		computeShader = ShaderManager::instance()->compileShader(ShaderType::COMPUTE,
			"C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/D3D11/compute/ComputeShader.hlsl");

		TEXTURE_DESC desc2;
		desc2.height = 256;
		desc2.width  = 256;
		desc2.unorderedAccess = true;
		desc2.format = TextureFormat::FORMAT_R32G32B32A32;

		tex = TextureManager::instance()->Create2dTexture(&desc2);
	}
};