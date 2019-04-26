#pragma once
#include "Hollow/Graphics/Renderer/Base/IMaterial.h"
#include "D3D11Texture.h"
#include "D3D11VertexShader.h"
#include "D3D11PixelShader.h"
#include "Hollow/Resources/ShaderManager.h"

class D3D11Material : public IMaterial
{
public:
	std::shared_ptr<D3DTexture> diffuseTexture;
	std::shared_ptr<D3DTexture> normalTexture;
	std::shared_ptr<D3DTexture> specularTexture;
	D3DPixelShader* pixelShader;
	D3D11VertexShader* vertexShader;
public:
	D3D11Material()
	{
		vertexShader = ShaderManager::instance()->getVertexShader("vs");
		pixelShader = ShaderManager::instance()->getPixelShader("ps");
	}

	void SetDiffuseTexture(D3DTexture* texture) { diffuseTexture = std::make_shared<D3DTexture>(*texture); }
	void SetNormalTexture(D3DTexture* texture) { normalTexture = std::make_shared<D3DTexture>(*texture); }
	void SetSpecularTexture(D3DTexture* texture) { specularTexture = std::make_shared<D3DTexture>(*texture); }
};