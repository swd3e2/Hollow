#pragma once

#include "Hollow/Graphics/ShaderManager.h"
#include <unordered_map>
#include <Hollow/Events/IEventListener.h>
#include <Hollow/Events/EventSystem.h>
#include "Sandbox/Events/FileChangeEvent.h"
#include "Hollow/Common/Log.h"

class ShaderManager : public Hollow::IEventListener
{
public:
	std::string baseShaderFolder;
	std::unordered_map<std::string, Hollow::s_ptr<Hollow::Shader>> shaders;
public:
	ShaderManager()
	{
		baseShaderFolder = "C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders";
		Hollow::EventSystem::instance()->addEventListener(this, &ShaderManager::update, FileChangeEvent::staticGetId());
	}

	void update(Hollow::IEvent* event) 
	{
		FileChangeEvent* changeEvent = static_cast<FileChangeEvent*>(event);
		//Hollow::ShaderManager::instance()->reload();
		HW_INFO("file has changed {}", changeEvent->filename.c_str());
	}

	Hollow::s_ptr<Hollow::Shader> create(const Hollow::SHADER_DESC& desc)
	{
		Hollow::s_ptr<Hollow::Shader> shader = Hollow::Shader::create(desc);
		if (desc.filename.size() > 0) {
			shaders[desc.filename] = shader;
		}
		return shader;
	}
};