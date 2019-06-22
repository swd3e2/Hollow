#pragma once

#ifndef HW_PROJECT_LOADER_H
#define HW_PROJECT_LOADER_H

#include <string>
#include "Hollow/Core/CModule.h"
#include "Hollow/Importer/gltf/json.hpp"
#include "Hollow/Common/FileSystem.h"
#include "Hollow/ECS/EntityManager.h"
#include "Hollow/ECS/ComponentManager.h"
#include "Hollow/Graphics/ShaderManager.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Components/RenderableComponent.h"

class ProjectSettings : public Hollow::CModule<ProjectSettings>
{
public:
	std::string MeshesFolder;
	std::string TexturesFolder;
	std::string ShadersFolder;

	bool isProjectLoaded = false;
public:
	ProjectSettings()
	{
		setStartedUp();
	}

	~ProjectSettings()
	{
		setShutdown();
	}

	void loadFromFile(std::string filename) 
	{
		using json = nlohmann::json;
			
		auto projectData = json::parse(Hollow::FileSystem::getFileContent(filename));

		Hollow::EntityManager::instance()->clear();
		Hollow::ComponentManager::instance()->clear();

		MeshesFolder = projectData["MeshesFolder"].get<std::string>();
		TexturesFolder = projectData["TexturesFolder"].get<std::string>();
		ShadersFolder = projectData["ShadersFolder"].get<std::string>();

		Hollow::ShaderManager::instance()->shaderTypeFolder = ShadersFolder;
		Hollow::ShaderManager::instance()->loadShadersFromFolder();

		for (auto& it : projectData["Entities"]) {
			GameObject* gameObject = Hollow::EntityManager::instance()->createEntity<GameObject>();

			if (it.find("TransformComponent") != it.end()) {
				TransformComponent* transform = gameObject->addComponent<TransformComponent>();
				transform->position = Hollow::Vector3(
					it["TransformComponent"]["translation"][0].get<float>(), 
					it["TransformComponent"]["translation"][1].get<float>(), 
					it["TransformComponent"]["translation"][2].get<float>()
				);
				transform->scale = Hollow::Vector3(
					it["TransformComponent"]["scale"][0].get<float>(),
					it["TransformComponent"]["scale"][1].get<float>(),
					it["TransformComponent"]["scale"][2].get<float>()
				);
				transform->rotation = Hollow::Vector3(
					it["TransformComponent"]["rotation"][0].get<float>(),
					it["TransformComponent"]["rotation"][1].get<float>(),
					it["TransformComponent"]["rotation"][2].get<float>()
				);
			}

			if (it.find("RenderableComponent") != it.end()) {
				RenderableComponent* transform = gameObject->addComponent<RenderableComponent, std::string>(MeshesFolder + "/" + it["RenderableComponent"]["filename"].get<std::string>());
			}
		}

		isProjectLoaded = true;
	}
};

#endif