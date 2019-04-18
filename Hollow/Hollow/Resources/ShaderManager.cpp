#include "ShaderManager.h"

void ShaderManager::startUp(ID3D11Device * device)
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

void ShaderManager::shutdown()
{
	setShutdown();
}

void ShaderManager::addVertexShader(std::string name, std::string shaderPath)
{
	if (m_vertexShaders.find(name) != m_vertexShaders.end())
	{
		m_vertexShaders[name] = new D3DVertexShader(device, shaderPath);
	}
}

D3DVertexShader * ShaderManager::getVertexShader(std::string name)
{
	if (m_vertexShaders.find(name) != m_vertexShaders.end())
	{
		return m_vertexShaders[name];
	}
	return nullptr;
}

void ShaderManager::addPixelShader(std::string name, std::string shaderPath)
{
	if (m_pixelShaders.find(name) != m_pixelShaders.end())
	{
		m_pixelShaders[name] = new D3DPixelShader(device, shaderPath);
	}
}

D3DPixelShader * ShaderManager::getPixelShader(std::string name)
{
	if (m_pixelShaders.find(name) != m_pixelShaders.end())
	{
		return m_pixelShaders[name];
	}
	return nullptr;
}

void ShaderManager::addGeometryShader(std::string name, std::string shaderPath)
{
	if (m_geometryShaders.find(name) != m_geometryShaders.end())
	{
		m_geometryShaders[name] = new D3DGeometryShader(device, shaderPath);
	}
}

D3DGeometryShader * ShaderManager::getGeometryShader(std::string name)
{
	if (m_geometryShaders.find(name) != m_geometryShaders.end())
	{
		return m_geometryShaders[name];
	}
	return nullptr;
}
