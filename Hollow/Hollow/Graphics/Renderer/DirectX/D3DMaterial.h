#pragma once
#include "Hollow/Graphics/IMaterial.h"
#include "D3DTexture.h"
#include "D3DVertexShader.h"
#include "D3DPixelShader.h"
#include "Hollow/Resources/ShaderManager.h"

class D3DMaterial : public IMaterial
{
public:
	std::shared_ptr<D3DTexture> diffuseTexture;
	std::shared_ptr<D3DTexture> normalTexture;
	std::shared_ptr<D3DTexture> specularTexture;
	D3DPixelShader* pixelShader;
	D3DVertexShader* vertexShader;
public:
	D3DMaterial()
	{
		vertexShader = ShaderManager::instance()->getVertexShader("vs");
		pixelShader = ShaderManager::instance()->getPixelShader("ps");
	}

	void SetDiffuseTexture(D3DTexture* texture) { diffuseTexture = std::make_shared<D3DTexture>(*texture); }
	void SetNormalTexture(D3DTexture* texture) { normalTexture = std::make_shared<D3DTexture>(*texture); }
	void SetSpecularTexture(D3DTexture* texture) { specularTexture = std::make_shared<D3DTexture>(*texture); }
};