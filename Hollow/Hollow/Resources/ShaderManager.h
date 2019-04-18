#pragma once

#ifndef HW_SHADER_MANAGER_H
#define HW_SHADER_MANAGER_H

#include <unordered_map>
#include "Hollow/Graphics/DirectX/D3DVertexShader.h"
#include "Hollow/Graphics/DirectX/D3DPixelShader.h"
#include "Hollow/Graphics/DirectX/D3DGeometryShader.h"
#include "Hollow/Common/FileSystem.h"
#include "Hollow/Common/Helper.h"
#include "Hollow/Core/CModule.h"
#include "Hollow/Platform.h"

class HOLLOW_API ShaderManager : public CModule<ShaderManager>
{
private:
	std::unordered_map<std::string, D3DVertexShader*> m_vertexShaders;
	std::unordered_map<std::string, D3DPixelShader*> m_pixelShaders;
	std::unordered_map<std::string, D3DGeometryShader*> m_geometryShaders;

	Hollow::FileSystem fs;
	ID3D11Device* device;
public:
	void startUp(ID3D11Device* device);
	void shutdown();
	void addVertexShader(std::string name, std::string shaderPath);
	
	D3DVertexShader* getVertexShader(std::string name);
	/*
	 * todo: need return result of creating shader
	 */
	void addPixelShader(std::string name, std::string shaderPath);
	D3DPixelShader* getPixelShader(std::string name);
	void addGeometryShader(std::string name, std::string shaderPath);
	D3DGeometryShader* getGeometryShader(std::string name);
	

	inline std::unordered_map<std::string, D3DPixelShader*>* getPixelShaderList()
	{
		return &m_pixelShaders;
	}

	inline std::unordered_map<std::string, D3DVertexShader*>* getVertexShaderList()
	{
		return &m_vertexShaders;
	}
};

#endif