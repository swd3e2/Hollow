#pragma once

#ifndef HW_PROJECT_LOADER_H
#define HW_PROJECT_LOADER_H

#include <string>
#include "Hollow/Core/CModule.h"
#include "Hollow/Importer/gltf/json.hpp"
#include "Hollow/Common/FileSystem.h"
#include "Hollow/ECS/EntityManager.h"
#include "Hollow/ECS/ComponentManager.h"

namespace Hollow {
	class ProjectSettings : public CModule<ProjectSettings>
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
			
			isProjectLoaded = true;
			auto projectData = json::parse(FileSystem::getFileContent(filename));

			EntityManager::instance()->clear();
			ComponentManager::instance()->clear();

			MeshesFolder = projectData["MeshesFolder"].get<std::string>();
			TexturesFolder = projectData["TexturesFolder"].get<std::string>();
			ShadersFolder = projectData["ShadersFolder"].get<std::string>();
		}
	};
}

#endif