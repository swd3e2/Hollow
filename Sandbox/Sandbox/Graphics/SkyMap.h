#pragma once

#include <Hollow/Math/Matrix4.h>
#include <Hollow/Resources/Mesh/Mesh.h>
#include <Hollow/Graphics/Vertex.h>
#include <Hollow/Graphics/HardwareBufferManager.h>
#include <Hollow/Graphics/TextureManager.h>
#include <Hollow/Import/FreeImgImporter.h>
#include <Hollow/Graphics/ShaderManager.h>
#include <Hollow/Test.h>
#include <Hollow/Common/Log.h>
class SkyMap
{
public:
	int NumSphereVertices;
	int NumSphereFaces;
	Hollow::Matrix4 transform;

	Hollow::Mesh* mesh;
	const std::string filename = "Sandbox/Resources/Textures/Irradiance.tga";
	SkyMap()
	{
		Hollow::s_ptr<Hollow::Import::Texture> texture = Hollow::FreeImgImporter::instance()->import(filename.c_str());
		
		if (!texture) {
			HW_INFO("Cubemap texture not found, filename {}", filename.c_str());
		}		

		int xOffset = texture->width / 4;
		int yOffset = texture->height / 3;
		std::array<unsigned char*, 6> data;
		for (int i = 0; i < data.size(); i++) {
			data[i] = new unsigned char[xOffset * yOffset * 4];
		}

		unsigned char* textureData = (unsigned char*)texture->data.get();

		// forward 1 1
		// ? * xOffset * 4 - width | yOffset * desc->width * ? * 4 - plane
		for (int oy = 0; oy < yOffset; oy++) {
			for (int ox = 0; ox < xOffset; ox++) {
				data[4][ox * 4 + oy * yOffset * 4 + 0] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 0];
				data[4][ox * 4 + oy * yOffset * 4 + 1] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 1];
				data[4][ox * 4 + oy * yOffset * 4 + 2] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 2];
				data[4][ox * 4 + oy * yOffset * 4 + 3] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 3];
			}
		}

		// back  2 1
		for (int oy = 0; oy < yOffset; oy++) {
			for (int ox = 0; ox < xOffset; ox++) {
				data[5][ox * 4 + oy * yOffset * 4 + 0] = textureData[(ox * 4) + (3 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 0];
				data[5][ox * 4 + oy * yOffset * 4 + 1] = textureData[(ox * 4) + (3 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 1];
				data[5][ox * 4 + oy * yOffset * 4 + 2] = textureData[(ox * 4) + (3 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 2];
				data[5][ox * 4 + oy * yOffset * 4 + 3] = textureData[(ox * 4) + (3 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 3];
			}
		}

		// up 1 0
		for (int oy = 0; oy < yOffset; oy++) {
			for (int ox = 0; ox < xOffset; ox++) {
				data[2][ox * 4 + oy * yOffset * 4 + 0] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 0 * 4) + (oy * texture->width * 4) + 0];
				data[2][ox * 4 + oy * yOffset * 4 + 1] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 0 * 4) + (oy * texture->width * 4) + 1];
				data[2][ox * 4 + oy * yOffset * 4 + 2] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 0 * 4) + (oy * texture->width * 4) + 2];
				data[2][ox * 4 + oy * yOffset * 4 + 3] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 0 * 4) + (oy * texture->width * 4) + 3];
			}
		}

		// down 1 2
		for (int oy = 0; oy < yOffset; oy++) {
			for (int ox = 0; ox < xOffset; ox++) {
				data[3][ox * 4 + oy * yOffset * 4 + 0] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 2 * 4) + (oy * texture->width * 4) + 0];
				data[3][ox * 4 + oy * yOffset * 4 + 1] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 2 * 4) + (oy * texture->width * 4) + 1];
				data[3][ox * 4 + oy * yOffset * 4 + 2] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 2 * 4) + (oy * texture->width * 4) + 2];
				data[3][ox * 4 + oy * yOffset * 4 + 3] = textureData[(ox * 4) + (1 * xOffset * 4) + (yOffset * texture->width * 2 * 4) + (oy * texture->width * 4) + 3];
			}
		}

		// right 2 1
		for (int oy = 0; oy < yOffset; oy++) {
			for (int ox = 0; ox < xOffset; ox++) {
				data[0][ox * 4 + oy * yOffset * 4 + 0] = textureData[(ox * 4) + (2 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 0];
				data[0][ox * 4 + oy * yOffset * 4 + 1] = textureData[(ox * 4) + (2 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 1];
				data[0][ox * 4 + oy * yOffset * 4 + 2] = textureData[(ox * 4) + (2 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 2];
				data[0][ox * 4 + oy * yOffset * 4 + 3] = textureData[(ox * 4) + (2 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 3];
			}
		}

		// left 0 1
		for (int oy = 0; oy < yOffset; oy++) {
			for (int ox = 0; ox < xOffset; ox++) {
				data[1][ox * 4 + oy * yOffset * 4 + 0] = textureData[(ox * 4) + (0 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 0];
				data[1][ox * 4 + oy * yOffset * 4 + 1] = textureData[(ox * 4) + (0 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 1];
				data[1][ox * 4 + oy * yOffset * 4 + 2] = textureData[(ox * 4) + (0 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 2];
				data[1][ox * 4 + oy * yOffset * 4 + 3] = textureData[(ox * 4) + (0 * xOffset * 4) + (yOffset * texture->width * 1 * 4) + (oy * texture->width * 4) + 3];
			}
		}

		Hollow::TEXTURE_DESC desc;
		desc.width = texture->width / 4;
		desc.height = texture->height / 3;
		desc.format = Hollow::TextureFormat::TF_RGB8;
		desc.type = Hollow::TextureType::TT_TEXTURE_CUBE;

		Hollow::s_ptr<Hollow::Texture> cubemap = Hollow::Texture::create(desc);
		//cubemap->update(data.data());

		mesh = getCube();
		mesh->models[0]->material = new Hollow::Material();
		mesh->models[0]->material->diffuseTexture = cubemap;
	}
};