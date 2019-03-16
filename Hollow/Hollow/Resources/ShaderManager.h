#pragma once
#include <unordered_map>
#include "Hollow/Graphics/DirectX/D3DVertexShader.h"
#include "Hollow/Graphics/DirectX/D3DPixelShader.h"
#include "Hollow/Graphics/DirectX/D3DGeometryShader.h"
#include "Hollow/Common/FileSystem.h"
#include "Hollow/Common/Helper.h"
#include "Hollow/Core/CModule.h"

class ShaderManager : public CModule<ShaderManager>
{
private:
	std::unordered_map<std::string, D3DVertexShader*> m_vertexShaders;
	std::unordered_map<std::string, D3DPixelShader*> m_pixelShaders;
	std::unordered_map<std::string, D3DGeometryShader*> m_geometryShaders;

	Hollow::FileSystem fs;
	ID3D11Device* device;
public:
	void startUp(ID3D11Device* device)
	{
		this->device = device;
		std::vector<std::string>* shaders;

		shaders = fs.read_directory("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/vertex/");
		for (auto& it : *shaders)
			if (strcmp(it.c_str(), ".") != 0 && strcmp(it.c_str(), "..") != 0)
				m_vertexShaders[Hollow::Helper::trim_to_symbol(it.c_str(), '.')] = new D3DVertexShader(device, "C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/vertex/" + it);

		shaders = fs.read_directory("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/pixel/");
		for (auto& it : *shaders)
			if (strcmp(it.c_str(), ".") != 0 && strcmp(it.c_str(), "..") != 0)
				m_pixelShaders[Hollow::Helper::trim_to_symbol(it.c_str(), '.')] = new D3DPixelShader(device, "C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/pixel/" + it);

		shaders = fs.read_directory("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/geometry/");
		for (auto& it : *shaders)
			if (strcmp(it.c_str(), ".") != 0 && strcmp(it.c_str(), "..") != 0)
				m_geometryShaders[Hollow::Helper::trim_to_symbol(it.c_str(), '.')] = new D3DGeometryShader(device, "C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/geometry/" + it);
		setStartedUp();
	}

	void shutdown()
	{
		setShutdown();
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

	void addGeometryShader(std::string name, std::string shaderPath)
	{
		if (m_geometryShaders.find(name) != m_geometryShaders.end())
		{
			m_geometryShaders[name] = new D3DGeometryShader(device, shaderPath);
		}
	}

	D3DGeometryShader* getGeometryShader(std::string name)
	{
		if (m_geometryShaders.find(name) != m_geometryShaders.end())
		{
			return m_geometryShaders[name];
		}
		return nullptr;
	}

	std::unordered_map<std::string, D3DPixelShader*>* getPixelShaderList()
	{
		return &m_pixelShaders;
	}

	std::unordered_map<std::string, D3DVertexShader*>* getVertexShaderList()
	{
		return &m_vertexShaders;
	}
};