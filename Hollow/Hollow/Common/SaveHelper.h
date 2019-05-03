#pragma once

#ifndef HW_SAVE_HELPER
#define HW_SAVE_HELPER

#include "Hollow/Core/CModule.h"
#include "Hollow/ECS/ComponentManager.h"
#include "Hollow/ECS/EntityManager.h"
#include "Hollow/ECS/TransformComponent.h"
#include "Hollow/ECS/RenderableComponent.h"
#include "Hollow/ECS/GameObject.h"
#include "Hollow/Graphics/DirectX/D3DRenderer.h"
#include "Hollow/Resources/MeshManager.h"
#include "Hollow/Common/Log.h"
#include "Hollow/Platform.h"

#include <fstream>

struct SaveFileHeader
{
	int count;
};

struct SaveFileEntityHeader
{
	TransformComponent transform;
	int lenght;
};

class HOLLOW_API SaveHelper : public CModule<SaveHelper>
{
public:
	const char* saveFilename = "save.bin";
public:
	SaveHelper()
	{
	}

	~SaveHelper()
	{
	}

	void startUp()
	{
		setStartedUp();
		load();
	}

	void shutdown()
	{
		setShutdown();
		//save();
	}

	void save()
	{
		std::ofstream file(saveFilename, std::ios::binary);

		SaveFileHeader header;
		header.count = EntityManager::instance()->getContainer<GameObject>()->entityList.getSize();
		file.write((char*)&header, sizeof(SaveFileHeader));

		for (auto& it : EntityManager::instance()->getContainer<GameObject>()->entityList) {
			file.write((char*)it.getComponent<TransformComponent>(), sizeof(TransformComponent));

			RenderableComponent* rendereable = it.getComponent<RenderableComponent>();
			int length = rendereable->renderable.name.size() + 1;
			file.write((char*)&length, sizeof(int));
			char * filename = new char[length];
			strcpy_s(filename, length, rendereable->renderable.name.c_str());
			file.write(const_cast<char*>(filename), sizeof(char) * length);

			delete filename;
		}
		file.close();
	}

	void load()
	{
		std::ifstream file(saveFilename, std::ios::binary);
		if (!file)
		{
			HW_DEBUG("No save file found.");
			return;
		}
		SaveFileHeader header;
		file.read((char*)&header, sizeof(SaveFileHeader));

		EntityManager::instance()->getContainer<GameObject>()->entityList.clear();

		for (auto& it : EntityManager::instance()->getContainer<TransformComponent>()->entityList) {
			it.~TransformComponent();
		}
		ComponentManager::instance()->getContainer<TransformComponent>()->componentList.clear();
		for (auto& it : EntityManager::instance()->getContainer<RenderableComponent>()->entityList) {
			it.~RenderableComponent();
		}
		ComponentManager::instance()->getContainer<RenderableComponent>()->componentList.clear();

		for (int i = 0; i < header.count; i++) 
		{
			GameObject* entity = EntityManager::instance()->createEntity<GameObject>();
			TransformComponent* transform = entity->addComponent<TransformComponent>();
			file.read((char*)transform, sizeof(TransformComponent));
			int lenght = 0;
			file.read((char*)&lenght, sizeof(int));
			char* filename = new char[lenght];
			file.read((char*)filename, sizeof(char) * lenght);
			entity->addComponent<RenderableComponent, ID3D11Device*, ID3D11DeviceContext*, Mesh*>
				(
					D3DRenderer::instance()->getContext(),
					MeshManager::instance()->CreateMesh(filename, true)
				);
			delete filename;
		}
		
		file.close();
	}
};

#endif