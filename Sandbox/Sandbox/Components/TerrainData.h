#pragma once

#include <Hollow/ECS/Component.h>
#include <Hollow/Graphics/Vertex.h>
#include <vector>
#include <Hollow/Graphics/HardwareBufferManager.h>
#include <Hollow/Importer/FreeImgImporter.h>

class TerrainData : public Hollow::Component<TerrainData>
{
public:
	Hollow::VertexBuffer* vBuffer;
	Hollow::IndexBuffer* iBuffer;
	Hollow::Texture* tex;

	std::string filename;
	int denominator = 1;
public:
	TerrainData() = default;

	TerrainData(const std::string& filename) noexcept
	{
		load(filename);
	}

	void load(const std::string& filename)
	{
		this->filename = filename;

		Hollow::TEXTURE_DESC* desc = Hollow::FreeImgImporter::instance()->import(filename.c_str());
		unsigned char* data = (unsigned char*)desc->mInitialData;

		std::vector<Hollow::Vertex> vertices;
		std::vector<unsigned int> indices;

		
		int width = desc->width / denominator, height = desc->height / denominator;

		for (int i = 1; i < width - 2; i++) {
			for (int j = 1; j < height - 2; j++) {
				Hollow::Vector3 normal = calculateNormal(i, j, desc->width, data);
				Hollow::Vector3 normal1 = calculateNormal(i, j + 1, desc->width, data);
				Hollow::Vector3 normal2 = calculateNormal(i + 1, j, desc->width, data);
				Hollow::Vector3 normal3 = calculateNormal(i + 1, j + 1, desc->width, data);
				float h  = getHeight(i    , j    , desc->width, data) / 20.0f;
				float h1 = getHeight(i    , j + 1, desc->width, data) / 20.0f;
				float h2 = getHeight(i + 1, j    , desc->width, data) / 20.0f;
				float h3 = getHeight(i + 1, j + 1, desc->width, data) / 20.0f;


				vertices.push_back({ (float)i, h, (float)j, 0.0f, 0.0f, normal.x, normal.y, normal.z });
				vertices.push_back({ (float)i, h1, (float)j + 1, 1.0f, 0.0f, normal1.x, normal1.y, normal1.z });
				vertices.push_back({ (float)i + 1, h2, (float)j, 0.0f, 1.0f, normal2.x, normal2.y, normal2.z });

				vertices.push_back({ (float)i, h1, (float)j + 1, 1.0f, 0.0f, normal1.x, normal1.y, normal1.z });
				vertices.push_back({ (float)i + 1, h3, (float)j + 1, 1.0f, 1.0f, normal3.x, normal3.y, normal3.z });
				vertices.push_back({ (float)i + 1, h2, (float)j, 0.0f, 1.0f, normal2.x, normal2.y, normal2.z });
			}
		}

		tex = Hollow::TextureManager::instance()->createTextureFromFile("g.jpg", true);
		//tex = Hollow::TextureManager::instance()->createTextureFromFile("test.gif", true);

		vBuffer = Hollow::VertexBuffer::create({ vertices.data(), vertices.size(), sizeof(Hollow::Vertex) });
	}

	Hollow::Vector3 calculateNormal(int x, int z, int width, unsigned char* data)
	{
		float heightL = getHeight(x - 1, z, width, data);
		float heightR = getHeight(x + 1, z, width, data);
		float heightD = getHeight(x, z - 1, width, data);
		float heightU = getHeight(x, z + 1, width, data);

		return Hollow::Vector3::Normalize(Hollow::Vector3(heightL - heightR, 2.0f, heightD - heightU));
	}

	float getHeight(int x, int y, int width, unsigned char* data)
	{
		if (x <= 0 || y <= 0) return data[(width * 4) * 0 + 4 * 0];
		if (x * denominator >= width || y * denominator >= width) return data[(width * 4) * (width - 1) + 4 * (width - 1)];

		return data[(width * 4) * x * denominator + 4 * y * denominator];
	}
};