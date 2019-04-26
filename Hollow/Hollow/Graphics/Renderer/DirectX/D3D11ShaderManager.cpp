#include "D3D11ShaderManager.h"

void D3D11ShaderManager::startUp()
{
	D3D11RenderApi* r = static_cast<D3D11RenderApi*>(IRenderApi::instance());
	this->device = r->getContext()->device;

	std::vector<std::string>* shaders;

	shaders = fs.read_directory("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/vertex/");
	for (auto& it : *shaders)
		if (strcmp(it.c_str(), ".") != 0 && strcmp(it.c_str(), "..") != 0)
			m_vertexShaders[Hollow::Helper::trim_to_symbol(it.c_str(), '.')] = new D3D11VertexShader(device, "C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/vertex/" + it);

	shaders = fs.read_directory("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/pixel/");
	for (auto& it : *shaders)
		if (strcmp(it.c_str(), ".") != 0 && strcmp(it.c_str(), "..") != 0)
			m_pixelShaders[Hollow::Helper::trim_to_symbol(it.c_str(), '.')] = new D3D11PixelShader(device, "C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/pixel/" + it);

	shaders = fs.read_directory("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/geometry/");
	for (auto& it : *shaders)
		if (strcmp(it.c_str(), ".") != 0 && strcmp(it.c_str(), "..") != 0)
			m_geometryShaders[Hollow::Helper::trim_to_symbol(it.c_str(), '.')] = new D3D11GeometryShader(device, "C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders/geometry/" + it);
	setStartedUp();
}

void D3D11ShaderManager::shutdown()
{
	setShutdown();
}

void D3D11ShaderManager::addVertexShader(std::string name, std::string shaderPath)
{
	if (m_vertexShaders.find(name) != m_vertexShaders.end())
	{
		m_vertexShaders[name] = new D3D11VertexShader(device, shaderPath);
	}
}

D3D11VertexShader * D3D11ShaderManager::getVertexShader(std::string name)
{
	if (m_vertexShaders.find(name) != m_vertexShaders.end())
	{
		return m_vertexShaders[name];
	}
	return nullptr;
}

void D3D11ShaderManager::addPixelShader(std::string name, std::string shaderPath)
{
	if (m_pixelShaders.find(name) != m_pixelShaders.end())
	{
		m_pixelShaders[name] = new D3D11PixelShader(device, shaderPath);
	}
}

D3D11PixelShader * D3D11ShaderManager::getPixelShader(std::string name)
{
	if (m_pixelShaders.find(name) != m_pixelShaders.end())
	{
		return m_pixelShaders[name];
	}
	return nullptr;
}

void D3D11ShaderManager::addGeometryShader(std::string name, std::string shaderPath)
{
	if (m_geometryShaders.find(name) != m_geometryShaders.end())
	{
		m_geometryShaders[name] = new D3D11GeometryShader(device, shaderPath);
	}
}

D3D11GeometryShader * D3D11ShaderManager::getGeometryShader(std::string name)
{
	if (m_geometryShaders.find(name) != m_geometryShaders.end())
	{
		return m_geometryShaders[name];
	}
	return nullptr;
}
