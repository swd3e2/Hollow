#pragma once

#include <Hollow/ECS/Component.h>
#include <Hollow/Graphics/Vertex.h>
#include <vector>
#include <Hollow/Graphics/HardwareBufferManager.h>
#include <Hollow/Importer/FreeImgImporter.h>
#include "Hollow/Platform.h"

struct TerrainVertex
{
	Hollow::Vector3 pos;
	Hollow::Vector2 texCoord;
	Hollow::Vector3 normal;
	Hollow::Vector3 color;

	TerrainVertex(float px, float py, float pz, float tu, float tv) :
		pos(px, py, pz), texCoord(tu, tv)
	{}
	TerrainVertex(float px, float py, float pz, float nx, float ny, float nz, float tu, float tv, float r, float g, float b) :
		pos(px, py, pz), normal(nx, ny, nz), texCoord(tu, tv), color(b, g, r)
	{}
	TerrainVertex(const Hollow::Vector3& pos, const Hollow::Vector3& normal, const Hollow::Vector2& texCoord, const Hollow::Vector3& color) :
		TerrainVertex(pos.x, pos.y, pos.z, normal.x, normal.y, normal.z, texCoord.x, texCoord.y, color.x, color.y, color.z)
	{}
};

class TerrainData : public Hollow::Component<TerrainData>
{
public:
	Hollow::VertexBuffer* vBuffer;
	Hollow::IndexBuffer* iBuffer;
	Hollow::s_ptr<Hollow::Texture> tex;

	std::string filename;
	int denominator = 1;
	Hollow::s_ptr<Hollow::TextureData> desc;
	unsigned short* data;
	unsigned char* colorData;
public:
	TerrainData() = default;

	TerrainData(const std::string& filename) noexcept
	{
		load(filename);
	}

	void load(const std::string& filename)
	{
		this->filename = filename;

		if (0) {
			desc = Hollow::FreeImgImporter::instance()->import(filename.c_str(), false);
			data = (unsigned short*)desc->data.get();
		} else {
			desc = std::make_shared<Hollow::TextureData>();
			desc->width = 1025;
			desc->height = 1025;
			desc->pitch = 4;
			FILE* file = fopen("C:\\dev\\Hollow Engine\\Sandbox\\Sandbox\\Resources\\Textures\\heightmap.r16", "rb");
			data = new unsigned short[desc->width * desc->height];
			fread(data, sizeof(unsigned short), desc->width * desc->height, file);
		}
		
		Hollow::s_ptr<Hollow::TextureData> texData = Hollow::FreeImgImporter::instance()->import("C:\\dev\\Hollow Engine\\Sandbox\\Sandbox\\Resources\\Textures\\colormap.bmp", false);
		colorData = (unsigned char*)texData->data.get();

		std::vector<TerrainVertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Hollow::Vector3> positions;
		std::vector<Hollow::Vector3> tempNormals;
		std::vector<Hollow::Vector3> normals;
		
		int width = desc->width / denominator, height = desc->height / denominator;

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				positions.push_back({(float)i, getHeight(i, j) / 200.0f, (float)j});
			}
		}

		for (int i = 0; i < width - 1; i++) {
			for (int j = 0; j < height - 1; j++) {
				const Hollow::Vector3& first = positions[width * i + j];
				const Hollow::Vector3& second = positions[width * i + (j + 1)];
				const Hollow::Vector3& third = positions[width * (i + 1) + j];
				const Hollow::Vector3& fourth = positions[width * (i + 1) + (j + 1)];

				Hollow::Vector3 crossA = Hollow::Vector3::cross(first - second, first - third);
				Hollow::Vector3 crossB = Hollow::Vector3::cross(fourth - second, fourth - third);
				tempNormals.push_back(Hollow::Vector3::normalize(crossA));
				tempNormals.push_back(Hollow::Vector3::normalize(crossB));
			}
		}

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				const bool isFirstRow = i == 0;
				const bool isFirstColumn = j == 0;
				const bool isLastRow = i == height - 1;
				const bool isLastColumn = j == width - 1;

				Hollow::Vector3 normal;

				// Look for triangles to the upper-righ
				if (!isFirstRow && !isFirstColumn) {
					normal += tempNormals[((i - 1) * (height - 1) + (j - 1)) * 2];
				}
				// Look for triangles to the upper-righ
				if (!isFirstRow && !isLastColumn) {
					normal += tempNormals[((i - 1) * (height - 1) + j) * 2];
					normal += tempNormals[((i - 1) * (height - 1) + j) * 2 + 1];
				}
				// Look for triangle to the bottom-righ
				if (!isLastRow && !isLastColumn) {
					normal += tempNormals[(i * (height - 1) + j) * 2];
				}
				// Look for triangles to the bottom-righ
				if (!isLastRow && !isFirstColumn) {
					normal += tempNormals[(i * (height - 1) + (j - 1)) * 2];
					normal += tempNormals[(i * (height - 1) + (j - 1)) * 2 + 1];
				}

				normals.push_back(Hollow::Vector3::normalize(normal));
			}
		}
		
		for (int i = 1; i < width - 1; i++) {
			for (int j = 1; j < height - 1; j++) {
				const Hollow::Vector3& first = positions[width * i + j];
				const Hollow::Vector3& second = positions[width * i + (j + 1)];
				const Hollow::Vector3& third = positions[width * (i + 1) + j];
				const Hollow::Vector3& fourth = positions[width * (i + 1) + (j + 1)];

				vertices.push_back(TerrainVertex(
					positions[width * i + j],
					normals[i * width + j], 
					{ 0.0f, 0.0f }, 
					getColor(i, j))
				);
				vertices.push_back(
					TerrainVertex(positions[width * i + (j + 1)],
						normals[i * width + (j + 1)],
						{ 1.0f, 0.0f },
						getColor(i, j + 1))
				);
				vertices.push_back(TerrainVertex(
					positions[width * (i + 1) + j],
					normals[(i + 1) * width + j], 
					{ 0.0f, 1.0f },
					getColor(i + 1, j))
				);

				vertices.push_back(TerrainVertex(
					positions[width * i + (j + 1)],
					normals[i * width + (j + 1)], 
					{ 1.0f, 0.0f },
					getColor(i, j + 1))
				);

				vertices.push_back(TerrainVertex(
					positions[width * (i + 1) + (j + 1)],	
					normals[(i + 1) * width + (j + 1)], 
					{ 1.0f, 1.0f }, 
					getColor(i + 1, j + 1))
				);
				vertices.push_back(TerrainVertex(
					positions[width * (i + 1) + j],		
					normals[(i + 1) * width + j], 
					{ 0.0f, 1.0f }, 
					getColor(i + 1, j))
				);
			}
		}

		//for (int i = 1; i < width - 1; i++) {
		//	for (int j = 1; j < height - 1; j++) {
		//		Hollow::Vector3 normal = calculateNormal(i, j);
		//		Hollow::Vector3 normal1 = calculateNormal(i, j + 1);
		//		Hollow::Vector3 normal2 = calculateNormal(i + 1, j);
		//		Hollow::Vector3 normal3 = calculateNormal(i + 1, j + 1);

		//		float h  = getHeight(i    , j    ) / 600.0f;
		//		float h1 = getHeight(i    , j + 1) / 600.0f;
		//		float h2 = getHeight(i + 1, j    ) / 600.0f;
		//		float h3 = getHeight(i + 1, j + 1) / 600.0f;


		//		vertices.push_back({ (float)i,		h,	(float)j,		0.0f, 0.0f });
		//		vertices.push_back({ (float)i,		h1, (float)j + 1,	1.0f, 0.0f });
		//		vertices.push_back({ (float)i + 1,	h2, (float)j,		0.0f, 1.0f });

		//		vertices.push_back({ (float)i,		h1, (float)j + 1,	1.0f, 0.0f });
		//		vertices.push_back({ (float)i + 1,	h3, (float)j + 1,	1.0f, 1.0f });
		//		vertices.push_back({ (float)i + 1,	h2, (float)j,		0.0f, 1.0f });
		//	}
		//}

		//for (int i = 0; i < vertices.size() / 3; i++) {
		//	TerrainVertex& first = vertices[i*3];
		//	TerrainVertex& second = vertices[i * 3 + 1];
		//	TerrainVertex& third = vertices[i * 3 + 2];

		//	Hollow::Vector3 v1 = first.pos - second.pos;
		//	Hollow::Vector3 v2 = third.pos - second.pos;

		//	Hollow::Vector3 cross = Hollow::Vector3::Normalize(Hollow::Vector3::cross(v1, v2));
		//	//cross.x = -cross.x;
		//	cross.y = -cross.y;
		//	//cross.z = -cross.z;

		//	first.normal = cross;
		//	second.normal = cross;
		//	third.normal = cross;
		//}

		tex = Hollow::TextureManager::instance()->createTextureFromFile("g.jpg", true);
		//tex = Hollow::TextureManager::instance()->createTextureFromFile("test.gif", true);

		vBuffer = Hollow::VertexBuffer::create({ vertices.data(), vertices.size(), sizeof(TerrainVertex) });
	}

	Hollow::Vector3 getColor(int x, int y)
	{
		return Hollow::Vector3(
			colorData[x * 3076 + y * 3] / 255.0f, 
			colorData[x * 3076 + y * 3 + 1] / 255.0f,
			colorData[x * 3076 + y * 3 + 2] / 255.0f
		);
	}

	Hollow::Vector3 calculateNormal(int x, int z)
	{
		float heightL = getHeight(x - 1, z);
		float heightR = getHeight(x + 1, z);
		float heightD = getHeight(x, z - 1);
		float heightU = getHeight(x, z + 1);

		return Hollow::Vector3::normalize(Hollow::Vector3(heightL - heightR, 2.0f, heightD - heightU));
	}

	float getHeight(int x, int y)
	{
		if (x <= 0 || y <= 0) {
			return data[0];
		}

		if (x * denominator >= desc->height || y * denominator >= desc->width) {
			return data[(desc->width - 1) * (desc->height - 1) - 1];
		}

		return (float)data[desc->width * x * denominator + y * denominator];
	}
};