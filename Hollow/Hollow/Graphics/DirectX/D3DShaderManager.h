#pragma once
#include "D3DVertexShader.h"
#include "D3DPixelShader.h"
#include <unordered_map>
#include "Hollow/Utils/FileSystem.h"
#include "Hollow/Utils/Helper.h"

class D3DShaderManager
{
private:
	std::unordered_map<std::string, D3DVertexShader*> m_vertexShaders;
	std::unordered_map<std::string, D3DPixelShader*> m_pixelShaders;
	Hollow::FileSystem fs;
	ID3D11Device* device;
	static D3DShaderManager* _instance;
public:
	D3DShaderManager(ID3D11Device* device) :
		device(device)
	{
		if (_instance == nullptr)
			_instance = this;

		Initialize();
	}

	void Initialize()
	{
		std::vector<std::string>* shaders = fs.read_directory("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/vertex/");
		for (auto& it : *shaders)
			if (strcmp(it.c_str(), ".") != 0 && strcmp(it.c_str(), "..") != 0)
				m_vertexShaders[Hollow::Helper::trim_to_symbol(it.c_str(), '.')] = new D3DVertexShader(device, "C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/vertex/" + it);

		shaders = fs.read_directory("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/pixel/");
		for (auto& it : *shaders)
			if (strcmp(it.c_str(), ".") != 0 && strcmp(it.c_str(), "..") != 0)
				m_pixelShaders[Hollow::Helper::trim_to_symbol(it.c_str(), '.')] = new D3DPixelShader(device, "C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/pixel/" + it);
	}

	void addVertexShader(std::string name, std::string shaderPath)
	{
		if (m_vertexShaders.find(name) != m_vertexShaders.end())
		{
			m_vertexShaders[name] = new D3DVertexShader(device, shaderPath);
		}
	}

	D3DVertexShader* getVertexShader(std::string name)
	{
		if (m_vertexShaders.find(name) != m_vertexShaders.end())
		{
			return m_vertexShaders[name];
		}
		return nullptr;
	}

	void addPixelShader(std::string name, std::string shaderPath)
	{
		if (m_pixelShaders.find(name) != m_pixelShaders.end())
		{
			m_pixelShaders[name] = new D3DPixelShader(device, shaderPath);
		}
	}

	D3DPixelShader* getPixelShader(std::string name)
	{
		if (m_pixelShaders.find(name) != m_pixelShaders.end())
		{
			return m_pixelShaders[name];
		}
		return nullptr;
	}
};