#pragma once

#include <Hollow/ECS/Component.h>
#include <Hollow/Graphics/Vertex.h>
#include <vector>
#include <Hollow/Graphics/HardwareBufferManager.h>

class TerrainData : public Hollow::Component<TerrainData>
{
public:
	Hollow::VertexBuffer* vBuffer;
	Hollow::IndexBuffer* iBuffer;
public:
	TerrainData()
	{
		std::vector<Hollow::Vertex> vertices;
		std::vector<unsigned int> indices;

		int width = 128, height = 128;

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				Hollow::Vertex vertex;
				vertex.pos = Hollow::Vector3(i, 0, j);
				vertices.push_back(vertex);
			}
		}

		for (int i = 0; i < width - 1; i++) {
			for (int j = 0; j < height - 1; j++) {
				indices.push_back(i * width + j);
				indices.push_back(i * width + j + 1);
				indices.push_back((i + 1) * width + j);

				indices.push_back(i * width + j + 1);
				indices.push_back((i + 1) * width + j + 1);
				indices.push_back((i + 1) * width + j);
			}
		}

		vBuffer = Hollow::VertexBuffer::create({ vertices.data(), vertices.size(), sizeof(Hollow::Vertex)});
		iBuffer = Hollow::IndexBuffer::create({ indices.data(), indices.size(), Hollow::INDEX_FORMAT::UINT });
	}
};