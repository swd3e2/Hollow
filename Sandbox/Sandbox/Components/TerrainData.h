#pragma once

#include <Hollow/ECS/Component.h>
#include <Hollow/Graphics/Vertex.h>
#include <vector>
#include <Hollow/Graphics/HardwareBufferManager.h>
#include <Hollow/Importer/FreeImgImporter.h>

struct TerrainVertex
{
	Hollow::Vector3 pos;
	Hollow::Vector2 texCoord;
	Hollow::Vector3 normal;

	TerrainVertex(float px, float py, float pz, float tu, float tv) :
		pos(px, py, pz), texCoord(tu, tv)
	{

	}
};

class TerrainData : public Hollow::Component<TerrainData>
{
public:
	Hollow::VertexBuffer* vBuffer;
	Hollow::IndexBuffer* iBuffer;
	Hollow::Texture* tex;

	std::string filename;
	int denominator = 1;
	Hollow::TextureData* desc;
	unsigned short* data;
public:
	TerrainData() = default;

	TerrainData(const std::string& filename) noexcept
	{
		load(filename);
	}

	void load(const std::string& filename)
	{
		this->filename = filename;

		desc = Hollow::FreeImgImporter::instance()->import(filename.c_str(), false);
		data = (unsigned short*)*desc->data;

		std::vector<TerrainVertex> vertices;
		std::vector<unsigned int> indices;

		
		int width = desc->width / denominator, height = desc->height / denominator;

		for (int i = 1; i < width - 1; i++) {
			for (int j = 1; j < height - 1; j++) {
				Hollow::Vector3 normal = calculateNormal(i, j);
				Hollow::Vector3 normal1 = calculateNormal(i, j + 1);
				Hollow::Vector3 normal2 = calculateNormal(i + 1, j);
				Hollow::Vector3 normal3 = calculateNormal(i + 1, j + 1);

				float h  = getHeight(i    , j    ) / 600.0f;
				float h1 = getHeight(i    , j + 1) / 600.0f;
				float h2 = getHeight(i + 1, j    ) / 600.0f;
				float h3 = getHeight(i + 1, j + 1) / 600.0f;


				vertices.push_back({ (float)i,		h,	(float)j,		0.0f, 0.0f });
				vertices.push_back({ (float)i,		h1, (float)j + 1,	1.0f, 0.0f });
				vertices.push_back({ (float)i + 1,	h2, (float)j,		0.0f, 1.0f });

				vertices.push_back({ (float)i,		h1, (float)j + 1,	1.0f, 0.0f });
				vertices.push_back({ (float)i + 1,	h3, (float)j + 1,	1.0f, 1.0f });
				vertices.push_back({ (float)i + 1,	h2, (float)j,		0.0f, 1.0f });
				/*vertices.push_back({ (float)i,		h, (float)j,		0.0f, 0.0f, normal.x,  normal.y,  normal.z });
				vertices.push_back({ (float)i,		h1, (float)j + 1,	1.0f, 0.0f, normal1.x, normal1.y, normal1.z });
				vertices.push_back({ (float)i + 1,	h2, (float)j,		0.0f, 1.0f, normal2.x, normal2.y, normal2.z });

				vertices.push_back({ (float)i,		h1, (float)j + 1,	1.0f, 0.0f, normal1.x, normal1.y, normal1.z });
				vertices.push_back({ (float)i + 1,	h3, (float)j + 1,	1.0f, 1.0f, normal3.x, normal3.y, normal3.z });
				vertices.push_back({ (float)i + 1,	h2, (float)j,		0.0f, 1.0f, normal2.x, normal2.y, normal2.z });*/
			}
		}

		//for (int i = 1; i < width - 2; i++) {
		//	for (int j = 1; j < height - 2; j++) {
		//		float h = getHeight(i, j, desc->width, desc->height, data) / 5.0f;
		//		float h1 = getHeight(i, j + 1, desc->width, desc->height, data) / 5.0f;
		//		float h2 = getHeight(i + 1, j, desc->width, desc->height, data) / 5.0f;
		//		float h3 = getHeight(i + 1, j + 1, desc->width, desc->height, data) / 5.0f;


		//		vertices.push_back({ (float)i,		h,	(float)j,		0.0f, 0.0f });
		//		vertices.push_back({ (float)i,		h1, (float)j + 1,	1.0f, 0.0f });
		//		vertices.push_back({ (float)i + 1,	h2, (float)j,		0.0f, 1.0f });

		//		vertices.push_back({ (float)i,		h1, (float)j + 1,	1.0f, 0.0f });
		//		vertices.push_back({ (float)i + 1,	h3, (float)j + 1,	1.0f, 1.0f });
		//		vertices.push_back({ (float)i + 1,	h2, (float)j,		0.0f, 1.0f });
		//	}
		//}

		for (int i = 0; i < vertices.size() / 3; i++) {
			TerrainVertex& first = vertices[i*3];
			TerrainVertex& second = vertices[i * 3 + 1];
			TerrainVertex& third = vertices[i * 3 + 2];

			Hollow::Vector3 v1 = first.pos - second.pos;
			Hollow::Vector3 v2 = third.pos - second.pos;

			Hollow::Vector3 cross = Hollow::Vector3::Normalize(Hollow::Vector3::cross(v1, v2));
			//cross.x = -cross.x;
			cross.y = -cross.y;
			//cross.z = -cross.z;

			first.normal = cross;
			second.normal = cross;
			third.normal = cross;
		}

		tex = Hollow::TextureManager::instance()->createTextureFromFile("g.jpg", true);
		//tex = Hollow::TextureManager::instance()->createTextureFromFile("test.gif", true);

		vBuffer = Hollow::VertexBuffer::create({ vertices.data(), vertices.size(), sizeof(TerrainVertex) });
	}

	Hollow::Vector3 calculateNormal(int x, int z)
	{
		float heightL = getHeight(x - 1, z);
		float heightR = getHeight(x + 1, z);
		float heightD = getHeight(x, z - 1);
		float heightU = getHeight(x, z + 1);

		return Hollow::Vector3::Normalize(Hollow::Vector3(heightL - heightR, 2.0f, heightD - heightU));
	}

	float getHeight(int x, int y)
	{
		if (x <= 0 || y <= 0) {
			return data[0];
		}

		if (x * denominator >= desc->height || y * denominator >= desc->width) {
			return data[(desc->pitch / 2) * (desc->height - 1) - 1];
		}

		return (float)data[(desc->pitch / 2) * x * denominator + y * denominator];
	}
};