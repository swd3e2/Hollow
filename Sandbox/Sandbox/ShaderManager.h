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
	std::unordered_map<std::string, Hollow::s_ptr<Hollow::Shader>> vertexShaders;
	std::unordered_map<std::string, Hollow::s_ptr<Hollow::Shader>> pixelShaders;
public:
	ShaderManager()
	{
		Hollow::EventSystem::instance()->addEventListener(this, &update, FileChangeEvent::getStaticEventId());
	}

	void update(Hollow::IEvent* event) 
	{
		FileChangeEvent* changeEvent = static_cast<FileChangeEvent*>(event);
		HW_INFO("file has changed {}", changeEvent->filename.c_str());
	}

	Hollow::s_ptr<Hollow::Shader> create(const Hollow::SHADER_DESC& desc)
	{
		Hollow::s_ptr<Hollow::Shader> shader = Hollow::Shader::create(desc);

		if (desc.type == Hollow::ShaderType::ST_VERTEX) {
			vertexShaders[desc.filename] = shader;
		} else if (desc.type == Hollow::ShaderType::ST_PIXEL) {
			pixelShaders[desc.filename] = shader;
		}
	}
};