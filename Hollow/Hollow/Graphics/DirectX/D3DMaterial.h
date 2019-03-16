#pragma once
#include "Hollow/Graphics/IMaterial.h"
#include "D3DTexture.h"
#include "D3DVertexShader.h"
#include "D3DPixelShader.h"
#include "Hollow/Resources/ShaderManager.h"

class D3DMaterial : public IMaterial
{
public:
	void SetDiffuseTexture(D3DTexture* texture) { diffuseTexture = texture; }
	void SetNormalTexture(D3DTexture* texture) { normalTexture = texture; }
	D3DTexture* diffuseTexture;
	D3DTexture* normalTexture;
	D3DPixelShader* pixelShader;
	D3DVertexShader* vertexShader;
	
	D3DMaterial()
	{
		vertexShader = ShaderManager::instance()->getVertexShader("vs");
		pixelShader = ShaderManager::instance()->getPixelShader("ps");
	}
};