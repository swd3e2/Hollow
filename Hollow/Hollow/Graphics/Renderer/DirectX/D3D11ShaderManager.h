#pragma once

#ifndef HW_SHADER_MANAGER_H
#define HW_SHADER_MANAGER_H

#include <unordered_map>
#include "D3D11VertexShader.h"
#include "D3D11PixelShader.h"
#include "D3D11GeometryShader.h"
#include "Hollow/Common/FileSystem.h"
#include "Hollow/Common/Helper.h"
#include "Hollow/Core/CModule.h"
#include "Hollow/Platform.h"
#include "D3D11RenderApi.h"

class HOLLOW_API D3D11ShaderManager : public CModule<D3D11ShaderManager>
{
private:
	std::unordered_map<std::string, D3D11VertexShader*> m_vertexShaders;
	std::unordered_map<std::string, D3D11PixelShader*> m_pixelShaders;
	std::unordered_map<std::string, D3D11GeometryShader*> m_geometryShaders;

	Hollow::FileSystem fs;
	ID3D11Device* device;
public:
	void startUp();
	void shutdown();
	void addVertexShader(std::string name, std::string shaderPath);
	
	D3D11VertexShader* getVertexShader(std::string name);
	/*
	 * todo: need return result of creating shader
	 */
	void addPixelShader(std::string name, std::string shaderPath);
	D3D11PixelShader* getPixelShader(std::string name);
	void addGeometryShader(std::string name, std::string shaderPath);
	D3D11GeometryShader* getGeometryShader(std::string name);
	

	inline std::unordered_map<std::string, D3D11PixelShader*>* getPixelShaderList()
	{
		return &m_pixelShaders;
	}

	inline std::unordered_map<std::string, D3D11VertexShader*>* getVertexShaderList()
	{
		return &m_vertexShaders;
	}
};

#endif