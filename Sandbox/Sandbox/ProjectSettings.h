#pragma once

#ifndef HW_PROJECT_LOADER_H
#define HW_PROJECT_LOADER_H

#include <string>
#include "Hollow/Core/CModule.h"
#include "vendor/tinygltf/json.hpp"
#include "Hollow/Common/FileSystem.h"
#include "Hollow/ECS/EntityManager.h"
#include "Hollow/ECS/ComponentManager.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Components/RenderableComponent.h"
#include "Sandbox/Entities/GameObject.h"
#include "Hollow/Core/DelayedTaskManager.h"
#include "Sandbox//Entities/Terrain.h"
#include "Sandbox/Components/TerrainData.h"
#include "Sandbox/Entities/Light.h"
#include "Sandbox/Components/LightComponent.h"

/**
 * Project settings class
 */
class ProjectSettings : public Hollow::CModule<ProjectSettings>
{
public:
	/** Project folders where app will find files */
	std::string MeshesFolder;
	std::string TexturesFolder;

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
			projectFileName = filename;

			for (auto& it : projectData["GameObjects"]) {
				GameObject* gameObject = Hollow::EntityManager::instance()->create<GameObject>();
				if (it.find("name") != it.end()) {
					gameObject->name = it["name"].get<std::string>();
				}

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
						)
					);
				}

				if (it.find("RenderableComponent") != it.end()) {
					gameObject->addComponent<RenderableComponent>(it["RenderableComponent"]["filename"].get<std::string>());
				}
			}

			for (auto& it : projectData["Terrain"]) {
				Terrain* terrain = Hollow::EntityManager::instance()->create<Terrain>();

				if (it.find("TransformComponent") != it.end()) {
					TransformComponent* transform = terrain->addComponent<TransformComponent>(
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
						)
					);
				}

				if (it.find("TerrainData") != it.end()) {
					terrain->addComponent<TerrainData>(it["TerrainData"]["filename"].get<std::string>());
				}
			}

			for (auto& it : projectData["Light"]) {
				Light* terrain = Hollow::EntityManager::instance()->create<Light>();
				if (it.find("LightComponent") != it.end()) {
					LightComponent* light = terrain->addComponent<LightComponent>();
					light->lightData.position = Hollow::Vector3(
						it["LightComponent"]["position"][0].get<float>(),
						it["LightComponent"]["position"][1].get<float>(),
						it["LightComponent"]["position"][2].get<float>()
					);
					light->lightData.direction = Hollow::Vector3(
						it["LightComponent"]["direction"][0].get<float>(),
						it["LightComponent"]["direction"][1].get<float>(),
						it["LightComponent"]["direction"][2].get<float>()
					);
					light->lightData.color = Hollow::Vector3(
						it["LightComponent"]["color"][0].get<float>(),
						it["LightComponent"]["color"][1].get<float>(),
						it["LightComponent"]["color"][2].get<float>()
					);

					light->lightData.constant	= it["LightComponent"]["constant"].get<float>();
					light->lightData.linear		= it["LightComponent"]["linear"].get<float>();
					light->lightData.quadratic	= it["LightComponent"]["quadratic"].get<float>();
					light->lightData.cutoff		= it["LightComponent"]["cutoff"].get<float>();
					light->lightData.distance	= it["LightComponent"]["distance"].get<float>();
					light->lightData.type		= it["LightComponent"]["type"].get<float>();
				}

				if (it.find("TerrainData") != it.end()) {
					terrain->addComponent<TerrainData>(it["TerrainData"]["filename"].get<std::string>());
				}
			}

			projectFolder = Hollow::Helper::trimToLastLineEntry(filename.c_str(), '\\');

			isProjectLoaded = true;
		};

		Hollow::DelayedTaskManager::instance()->add(func);
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

			Hollow::FileSystem::createFolder(projectFolder);
			Hollow::FileSystem::createFolder(projectFolder + "/Meshes");
			Hollow::FileSystem::createFolder(projectFolder + "/Textures");

			json projectData;

			MeshesFolder = projectData["MeshesFolder"] = projectFolder + "/Meshes";
			TexturesFolder = projectData["TexturesFolder"] = projectFolder + "/Textures";

			Hollow::FileSystem::writeToFile(projectFileName, projectData.dump(2).c_str());

			isProjectLoaded = true;
		};

		Hollow::DelayedTaskManager::instance()->add(func);
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

			int counter = 0;
			for (auto& it : Hollow::EntityManager::instance()->container<GameObject>()) {
				projectData["GameObjects"][counter]["id"] = it.getId();
				projectData["GameObjects"][counter]["name"] = it.name.c_str();

				if (it.hasComponent<TransformComponent>()) {
					TransformComponent* transform = it.getComponent<TransformComponent>();

					projectData["GameObjects"][counter]["TransformComponent"] = {
						{ "rotation", { transform->rotation.x, transform->rotation.y, transform->rotation.z }},
						{ "translation", { transform->position.x, transform->position.y, transform->position.z }},
						{ "scale", { transform->scale.x, transform->scale.y, transform->scale.z }}
					};
				}

				if (it.hasComponent<RenderableComponent>()) {
					projectData["GameObjects"][counter]["RenderableComponent"] = {
						{ "filename", it.getComponent<RenderableComponent>()->filename}
					};
				}
				counter++;
			}

			counter = 0;
			for (auto& it : Hollow::EntityManager::instance()->container<Terrain>()) {
				if (it.hasComponent<TransformComponent>()) {
					TransformComponent* transform = it.getComponent<TransformComponent>();

					projectData["Terrain"][counter]["TransformComponent"] = {
						{ "rotation", { transform->rotation.x, transform->rotation.y, transform->rotation.z }},
						{ "translation", { transform->position.x, transform->position.y, transform->position.z }},
						{ "scale", { transform->scale.x, transform->scale.y, transform->scale.z }}
					};
				}

				if (it.hasComponent<TerrainData>()) {
					projectData["Terrain"][counter]["TerrainData"] = {
						{ "filename", it.getComponent<TerrainData>()->filename}
					};
				}
			}

			counter = 0;
			for (auto& it : Hollow::EntityManager::instance()->container<Light>()) {
				if (it.hasComponent<LightComponent>()) {
					LightComponent* light = it.getComponent<LightComponent>();

					projectData["Light"][counter]["LightComponent"] = {
						{ "position", { light->lightData.position.x, light->lightData.position.y, light->lightData.position.z }},
						{ "constant", light->lightData.constant },
						{ "direction", { light->lightData.direction.x, light->lightData.direction.y, light->lightData.direction.z }},
						{ "linear", light->lightData.linear },
						{ "color", { light->lightData.color.x, light->lightData.color.y, light->lightData.color.z }},
						{ "quadratic", light->lightData.quadratic },
						{ "cutoff", light->lightData.cutoff },
						{ "distance", light->lightData.distance },
						{ "type", light->lightData.type },
					};
				}
			}

			Hollow::FileSystem::writeToFile(projectFileName, projectData.dump(2).c_str());
		};

		Hollow::DelayedTaskManager::instance()->add(func);
	}

	Hollow::RendererType getRendererType() const { return rendererType; }
private:
};

#endif