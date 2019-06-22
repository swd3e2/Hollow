#pragma once

#include "Hollow/ECS/Component.h"
#include "Hollow/Graphics/Renderer/Base/VertexBuffer.h"
#include "Hollow/Graphics/Renderer/Base/IndexBuffer.h"
#include <string>
#include <fstream>
#include "Hollow/Graphics/Vertex.h"
#include "Hollow/Importer/gltf/json.hpp"
#include "Hollow/Common/FileSystem.h"
#include "Hollow/Graphics/HardwareBufferManager.h"

using json = nlohmann::json;

struct RenderableObject {
	Hollow::VertexBuffer* vBuffer;
	Hollow::IndexBuffer* iBuffer;
	int material;
};

class RenderableComponent : public Hollow::Component<RenderableComponent>
{
public:
	std::vector<RenderableObject> renderables;
public:
	RenderableComponent(const std::string& filename)
	{
		using namespace Hollow;

		auto j = json::parse(FileSystem::getFileContent(filename));

		std::string filename = j["Data"]["filename"].get<std::string>();
		std::ifstream file(filename, std::ios::binary);

		for (int i = 0; i < j["Meshes"].size(); i++) {
			RenderableObject renderable;
			renderable.material = j["Meshes"][i]["material"].get<int>();

			file.seekg(j["Meshes"][i]["vertices_offset"].get<size_t>(), std::fstream::beg);
			Hollow::Vertex* vertices = new Hollow::Vertex[j["Meshes"][i]["vertices_size"].get<size_t>()];
			file.read((char*)vertices, sizeof(Hollow::Vertex) * j["Meshes"][i]["vertices_size"].get<size_t>());

			renderable.vBuffer = HardwareBufferManager::instance()->createVertexBuffer(
				vertices, 
				j["Meshes"][i]["vertices_size"].get<size_t>()
			);

			file.seekg(j["Meshes"][i]["indices_offset"].get<size_t>(), std::fstream::beg);
			unsigned int* indices = new unsigned int[j["Meshes"][i]["indices_size"].get<size_t>()];
			file.read((char*)indices, sizeof(unsigned int) * j["Meshes"][i]["indices_size"].get<int>());

			renderable.iBuffer = HardwareBufferManager::instance()->createIndexBuffer(
				indices,
				j["Meshes"][i]["indices_size"].get<int>()
			);

			renderables.push_back(renderable);
			delete[] vertices;
			delete[] indices;
		}

		file.close();

	}
};