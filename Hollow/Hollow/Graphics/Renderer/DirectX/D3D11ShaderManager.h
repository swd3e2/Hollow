#pragma once

#ifndef HW_D3D11_SHADER_MANAGER_H
#define HW_D3D11_SHADER_MANAGER_H

#include "Hollow/Common/FileSystem.h"
#include "Hollow/Common/Helper.h"
#include "D3D11Prerequisites.h"
#include "Hollow/Graphics/ShaderManager.h"
#include "D3D11Shader.h"

class D3D11ShaderManager : public ShaderManager
{
private:
	Hollow::FileSystem fs;
	ID3D11Device* device;
public:
	D3D11ShaderManager();
	~D3D11ShaderManager();
};

#endif