#pragma once

#ifndef HW_PROJECT_LOADER_H
#define HW_PROJECT_LOADER_H

#include <string>
#include "Hollow/Core/CModule.h"
#include "vendor/tinygltf/json.hpp"
#include "Hollow/Common/FileSystem.h"
#include "Hollow/ECS/EntityManager.h"
#include "Hollow/ECS/ComponentManager.h"
#include "Hollow/Graphics/ShaderManager.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Components/RenderableComponent.h"
#include "Sandbox/Entities/GameObject.h"
#include "Hollow/Core/DelayedTaskManager.h"

/**
 * Project settings class
 */
class ProjectSettings : public Hollow::CModule<ProjectSettings>
{
public:
	/** Project folders where app will find files */
	std::string MeshesFolder;
	std::string TexturesFolder;
	std::string ShadersFolder;

	/** Project folders */
	std::string projectFolder;
	/** Project name */
	std::string projectName;
	/** Project settings filename for saving */
	std::string projectFileName;

	Hollow::RendererType rendererType;

	bool isProjectLoaded = false;
public:
	ProjectSettings(Hollow::RendererType type) : rendererType(type) {}
	/**
	 * Loads settings from json file
	 * @param[in] filename	Filename of settings file
	 */
	void load(std::string filename) 
	{
		auto func = [&, filename]() {
			using json = nlohmann::json;

			auto projectData = json::parse(Hollow::FileSystem::getFileContent(filename));

			/*Hollow::EntityManager::instance()->clear();
			Hollow::ComponentManager::instance()->clear();*/

			MeshesFolder = projectData["MeshesFolder"].get<std::string>();
			TexturesFolder = projectData["TexturesFolder"].get<std::string>();
			ShadersFolder = projectData["ShadersFolder"].get<std::string>();
			projectFileName = filename;

			Hollow::ShaderManager::instance()->loadShadersFromFolder("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders");

			for (auto& it : projectData["Entities"]) {
				GameObject* gameObject = Hollow::EntityManager::instance()->create<GameObject>();

				if (it.find("TransformComponent") != it.end()) {
					TransformComponent* transform = gameObject->addComponent<TransformComponent>(
					Hollow::Vector3(
						it["TransformComponent"]["translation"][0].get<float>(),
						it["TransformComponent"]["translation"][1].get<float>(),
						it["TransformComponent"]["translation"][2].get<float>()
					),
					Hollow::Vector3(
						it["TransformComponent"]["scale"][0].get<float>(),
						it["TransformComponent"]["scale"][1].get<float>(),
						it["TransformComponent"]["scale"][2].get<float>()
					),
					Hollow::Vector3(
						it["TransformComponent"]["rotation"][0].get<float>(),
						it["TransformComponent"]["rotation"][1].get<float>(),
						it["TransformComponent"]["rotation"][2].get<float>()
					));
				}

				if (it.find("RenderableComponent") != it.end()) {
					gameObject->addComponent<RenderableComponent>(it["RenderableComponent"]["filename"].get<std::string>());
				}
			}

			projectFolder = Hollow::Helper::trim_to_last_line_entry(filename.c_str(), '\\');

			isProjectLoaded = true;
		};

		Hollow::DelayedTaskManager::instance()->Add(func);
	}

	/**
	 * Creates new project
	 * @param[in] folder	Path to folder where project needs to be created
	 * @param[in] name		Name of new project
	 */
	void create(std::string folder, std::string name)
	{
		auto func = [&, folder, name]() {
			using json = nlohmann::json;

			if (folder[folder.size() - 1] != '/') {
				projectFolder = folder + "/" + name;
			} else {
				projectFolder = folder + name;
			}

			projectName = name;
			projectFileName = projectFolder + "/" + name + ".json";

			Hollow::FileSystem::CreateFolder(projectFolder);
			Hollow::FileSystem::CreateFolder(projectFolder + "/Meshes");
			Hollow::FileSystem::CreateFolder(projectFolder + "/Textures");
			Hollow::FileSystem::CreateFolder(projectFolder + "/Shaders");

			json projectData;

			MeshesFolder = projectData["MeshesFolder"] = projectFolder + "/Meshes";
			TexturesFolder = projectData["TexturesFolder"] = projectFolder + "/Textures";
			ShadersFolder = projectData["ShadersFolder"] = projectFolder + "/Shaders";

			copyShaders();
			Hollow::ShaderManager::instance()->loadShadersFromFolder(ShadersFolder);

			Hollow::FileSystem::writeToFile(projectFileName, projectData.dump(2).c_str());

			isProjectLoaded = true;
		};

		Hollow::DelayedTaskManager::instance()->Add(func);
	}

	/**
	 * Saves project to project settings file
	 */
	void save()
	{
		auto func = [&]() {
			using json = nlohmann::json;
			if (!projectFolder.size() && isProjectLoaded) {
				return;
			}

			json projectData;

			projectData["MeshesFolder"] = MeshesFolder;
			projectData["TexturesFolder"] = TexturesFolder;
			projectData["ShadersFolder"] = ShadersFolder;

			int counter = 0;
			for (auto& it : Hollow::EntityManager::instance()->container<GameObject>()) {
				projectData["Entities"][counter]["id"] = it.getId();
				if (it.hasComponent<TransformComponent>()) {
					TransformComponent* transform = it.getComponent<TransformComponent>();

					projectData["Entities"][counter]["TransformComponent"] = {
						{"rotation", { transform->rotation.x, transform->rotation.y, transform->rotation.z }},
						{"translation", { transform->position.x, transform->position.y, transform->position.z }},
						{"scale", { transform->scale.x, transform->scale.y, transform->scale.z }}
					};
				}

				if (it.hasComponent<RenderableComponent>()) {
					RenderableComponent* renderable = it.getComponent<RenderableComponent>();

					projectData["Entities"][counter]["RenderableComponent"] = {
						{"filename", renderable->filename}
					};
				}
				counter++;
			}

			Hollow::FileSystem::writeToFile(projectFileName, projectData.dump(2).c_str());
		};

		Hollow::DelayedTaskManager::instance()->Add(func);
	}

	Hollow::RendererType getRendererType() const { return rendererType; }
private:
	void copyShaders()
	{
		Hollow::FileSystem::CreateFolder(ShadersFolder + "/D3D11");
		Hollow::FileSystem::CreateFolder(ShadersFolder + "/OGL");

		Hollow::FileSystem::CreateFolder(ShadersFolder + "/D3D11/vertex");
		Hollow::FileSystem::CreateFolder(ShadersFolder + "/D3D11/pixel");
		Hollow::FileSystem::CreateFolder(ShadersFolder + "/OGL/vertex");
		Hollow::FileSystem::CreateFolder(ShadersFolder + "/OGL/pixel");

		Hollow::FileSystem::Copy("Sandbox/Resources/Shaders/D3D11/vertex/default.hlsl", ShadersFolder + "/D3D11/vertex/default.hlsl");
		Hollow::FileSystem::Copy("Sandbox/Resources/Shaders/D3D11/pixel/default.hlsl", ShadersFolder + "/D3D11/pixel/default.hlsl");
		Hollow::FileSystem::Copy("Sandbox/Resources/Shaders/D3D11/vertex/SkyMap.hlsl", ShadersFolder + "/D3D11/vertex/SkyMap.hlsl");
		Hollow::FileSystem::Copy("Sandbox/Resources/Shaders/D3D11/pixel/SkyMap.hlsl", ShadersFolder + "/D3D11/pixel/SkyMap.hlsl");
		Hollow::FileSystem::Copy("Sandbox/Resources/Shaders/D3D11/vertex/picker.hlsl", ShadersFolder + "/D3D11/vertex/picker.hlsl");
		Hollow::FileSystem::Copy("Sandbox/Resources/Shaders/D3D11/pixel/picker.hlsl", ShadersFolder + "/D3D11/pixel/picker.hlsl");

		Hollow::FileSystem::Copy("Sandbox/Resources/Shaders/OGL/vertex/default.glsl", ShadersFolder + "/OGL/vertex/default.glsl");
		Hollow::FileSystem::Copy("Sandbox/Resources/Shaders/OGL/pixel/default.glsl", ShadersFolder + "/OGL/pixel/default.glsl");
		Hollow::FileSystem::Copy("Sandbox/Resources/Shaders/OGL/vertex/SkyMap.glsl", ShadersFolder + "/OGL/vertex/SkyMap.glsl");
		Hollow::FileSystem::Copy("Sandbox/Resources/Shaders/OGL/pixel/SkyMap.glsl", ShadersFolder + "/OGL/pixel/SkyMap.glsl");
		Hollow::FileSystem::Copy("Sandbox/Resources/Shaders/OGL/vertex/picker.glsl", ShadersFolder + "/OGL/vertex/picker.glsl");
		Hollow::FileSystem::Copy("Sandbox/Resources/Shaders/OGL/pixel/picker.glsl", ShadersFolder + "/OGL/pixel/picker.glsl");
	}
};

#endif